/* wakeup.c - wakeup */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include "lab6_h.h"

/*------------------------------------------------------------------------
 *  wakeup  --  called by clock interrupt dispatcher to awaken processes
 *------------------------------------------------------------------------
 */

SYSCALL wakeupmem()
{
    int ind, retval;
    word f, m;

    while (q[slp_head].qnext != slp_tail && (firstkey(slp_head) <= 0))
    {
        if (sleepmemt(ind = q_to_tab(dequeue(firstid(slp_head)))) == SYSERR)
        {
            retval = freep(slpMemtab[ind].ptr);
            slpMemtab[ind].ptr = NULL;
            return retval;
        }
    }
             
    if ((slp_empty = (q[slp_head].qnext != slp_tail)) != 0)
        slp_top = &firstkey(slp_head);
} /* wakeup */



int q_to_tab(ind)
int ind;
{
    int nqent = NQENT;
    return ind - nqent;
}


int tab_to_q(ind)
int ind;
{
    int nqent = NQENT;
    return ind + nqent;
}
