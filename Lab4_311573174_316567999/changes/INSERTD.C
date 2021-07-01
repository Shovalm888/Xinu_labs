/* insertd.c - insertd */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include "lab6_h.h"

/*------------------------------------------------------------------------
 *  insertd  --  insert process pid in delta list "head", given its key
 *------------------------------------------------------------------------
 */
INTPROC insertd(id, head, key)
	int	id;
	int	head;
	int	key;
{
	int	next;			/* runs through list		*/
	int	prev;			/* follows next through list	*/

	for(prev=head,next=q[head].qnext ;
	    q[next].qkey < key ; prev=next,next=q[next].qnext)
		key -= q[next].qkey;
	q[id].qnext = next;
	q[id].qprev = prev;
	q[id].qkey  = key;
	q[prev].qnext = id;
	q[next].qprev = id;
	//**LAB_6 :
	if (next < NPROC || (next < (NEWNQENT - 2) && next >= NQENT))
		q[next].qkey -= key;
	return(OK);
}
