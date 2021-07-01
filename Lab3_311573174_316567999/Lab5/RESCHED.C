
/* resched.c - resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
//**LAB5
#include "lab5.H"

/*------------------------------------------------------------------------
 *  resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRCURR.
 *------------------------------------------------------------------------
 */
//**LAB5 start
void get_max_ready() {

	int proc;
	max_peffec_pid = q[rdytail].qprev;
	max_peffec = q[max_peffec_pid].qkey;
	proc = q[rdyhead].qnext;
	while (proc != rdytail) {
		if (q[proc].qkey > max_peffec) {
			max_peffec = q[proc].qkey;
			max_peffec_pid = proc;
		}
	}
}
//**LAB5 end

int	resched()
{
	register struct	pentry* optr;	/* pointer to old process entry */
	register struct	pentry* nptr;	/* pointer to new process entry */

	optr = &proctab[currpid];

	//kprintf("\npid : %d\tcurr : %d\t ready : %d", getpid(), current_time[currpid], runnable_time[currpid]);
	if (optr->pstate == PRCURR)
	{
		/* no switch needed if current prio. higher than next	*/
		/* or if rescheduling is disabled ( pcxflag == 0 )	*/
		//kprintf("\npid: %d\tprio: %d\tlast: %d ",  getpid(), optr->pprio, lastkey(rdytail));
		//**LAB5 start
		if (sys_pcxget() == 0 || max_peffec < optr->pprio
			|| ((max_peffec == optr->pprio) && (preempt > 0)))
			return;
		//**LAB5 end
		/* force context switch */
		optr->pstate = PRREADY;
		insert(currpid, rdyhead, optr->pprio);
	} /* if */
	else if (sys_pcxget() == 0)
	{
		kprintf("pid=%d state=%d name=%s",
			currpid, optr->pstate, optr->pname);
		panic("Reschedule impossible in this state");
	} /* else if */

/* remove highest priority process at end of ready list */
	//**LAB5 start
	nptr = &proctab[(currpid = dequeue(max_peffec_pid))];
	max_peffec_pid = q[rdytail].qprev;
	max_peffec = q[max_peffec_pid].qkey;
	//**LAB5 end
	nptr->pstate = PRCURR;		/* mark it currently running	*/
	preempt = QUANTUM;		/* reset preemption counter	*/
	ctxsw(&optr->pregs, &nptr->pregs);
	/* The OLD process returns here when resumed. */
	return;
}

