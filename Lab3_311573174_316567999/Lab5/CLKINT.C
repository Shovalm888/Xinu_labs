
/* clkint.c - clkint */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <io.h>
#include <butler.h>
//**LAB5
#include "lab5.H"
#include <proc.h>
#include <q.h>

//**LAB5 start
void prio_validation(proc, newprio/*, is_ready*/) {

    if (newprio < 1)
        newprio = 1;

    else if (newprio >= BTLRPRIO)
        newprio--;

    //if (is_ready)
    q[proc].qkey = newprio;

    proctab[proc].pprio = newprio;
}

void prio_updating() {

    int newprio, proc;

    proc = q[rdyhead].qnext;
    while (proc != rdytail) {
        if (proc > 3) {
            runnable_time[proc]++;
            newprio = runnable_time[currpid] ? 1 + (pprio[currpid] * (runnable_time[currpid] - current_time[currpid])) / runnable_time[currpid]
		:proctab[currpid].pprio;
            prio_validation(proc, newprio);
        }
        if (q[proc].qkey > max_peffec) {
            max_peffec = q[proc].qkey;
            max_peffec_pid = proc;
        }
        proc = q[proc].qnext;
    }

    if (currpid > 3) {
        current_time[currpid]++;
        newprio = runnable_time[currpid] ? 1 + (pprio[currpid] * (runnable_time[currpid] - current_time[currpid])) / runnable_time[currpid]
		:proctab[currpid].pprio;
        prio_validation(currpid, newprio);
    }
    if (currpid == 0) current_time[0]++;
}
//**LAB5 end

/*------------------------------------------------------------------------
 *  clkint  --  clock service routine
 *  called at every clock tick and when starting the deferred clock
 *------------------------------------------------------------------------
 */
INTPROC clkint(mdevno)
int mdevno;				/* minor device number		*/
{
    int	i, pid;
    int resched_flag = 0;


    tod++;

    //**LAB5 start   
    prio_updating();
    //**LAB5 end

    if (slnempty)
        if ((-- * sltop) <= 0)
        {
            resched_flag = 1;
            wakeup();
        } /* if */
    if ((--preempt) <= 0)
        resched_flag = 1;

    if (resched_flag == 1)
        resched();

}
