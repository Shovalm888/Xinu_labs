/* sleept.c - sleept */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include "lab6_h.h"

/*------------------------------------------------------------------------
 *  sleept  --  delay the caller for a time specified in system ticks
 *------------------------------------------------------------------------
 */
SYSCALL	sleepmemt(ind)
int ind;
{
	int secs;
	secs = slpMemtab[ind].secs;
	if (secs <= 0)
		return(SYSERR);
	
	if (secs >= TICSD) {
		slpMemtab[ind].secs -= TICSD;
		secs = TICSN;
	}
	else
	{
		secs = (int)(((long)secs * (long)TICSN) / (long)TICSD);
		slpMemtab[ind].secs = 0;
	}
		
	insertd(tab_to_q(ind), slp_head, secs);
	slp_empty = TRUE;
	slp_top = & q[q[slp_head].qnext].qkey;
	return(OK);
}
