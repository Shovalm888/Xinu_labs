/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <q.h>
#include "lab6_h.h"

/*--------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *--------------------------------------------------------------------
 */
SYSCALL	freemem(block, size)
char *block;
word size;
{
	int	ps;
	struct	mblock	*p, *v;
	char	*top;
	int ind;

	if (!((block - 4) < end))
	{
		block -= 4; // to the 4 bytes we saved the len
		size = roundew(size + 4);
	}
	else
		size = roundew(size);
		
	block = (char *) truncew( (word)block );
	//kprintf("\n free mem - %d size : %u , max : %u   end: %u", block, size, (word)maxaddr, (word)end);
	if (size == 0 || block > maxaddr || (maxaddr - block) < size ||
		block < end) 
	{
		//kprintf("\nERR_1 -> block : %d, size : %u, by pid : %d", block, size, getpid());
		return(SYSERR);
	}
	disable(ps);

	for (v = &memlist, p = memlist.mnext;
		(char*)p != NULL && (char*)p < block;
		v = p, p = p->mnext);
	if (v != &memlist && (top=(char *)v+ v->mlen) > block
		 || (char *)p != NULL && (block+size) > (char *)p ) {
		restore(ps);
		//kprintf("\nERR_2 -> p : %d, v : %d , vlen : %u", p, v, v->mlen);
		return(SYSERR);
	}
	//**LAB_6:
	ind = *((word*)block + 1);
	if (ind >= 0 && ind < NUM_OF_SLPMEMS && slpMemtab[ind].ptr == (block + 4))
	{
		slpMemtab[ind].ptr = NULL;
		ind = tab_to_q(ind);
		if (q[ind].qnext != slp_tail) /* qnext is not tail */
			q[q[ind].qnext].qkey += q[ind].qkey;  /* Account for the
														  pid delay */
		dequeue(ind);
	}

	//**LAB_6 END
	if (v != &memlist && top == block )
		v->mlen += size;
	else {
		((struct mblock *)block)->mlen = size;
		((struct mblock *)block)->mnext = p;
		v->mnext = (struct mblock *)block;
		(char *)v = block;
	}
	if ( ((char *)v + v->mlen) == (char *)p ) {
		v->mlen += p->mlen;
		v->mnext = p->mnext;
	}
	restore(ps);
	return(OK);
}
