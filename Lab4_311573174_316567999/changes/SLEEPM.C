/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include"lab6_h.h"

/*------------------------------------------------------------------------
 *  sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */

SYSCALL getmemForTime(nbytes, secs, ptr)
int nbytes, secs;
void** ptr;
{
	int	ps, i;
	wait(meminsert_mutex);
	for (i = 0; i < NUM_OF_SLPMEMS && slpMemtab[i].ptr != NULL; i++);
	if (i == NUM_OF_SLPMEMS) 
	{
		kprintf("\nERR_1 sleepm");
		return(SYSERR);
	}
	if (secs <= 0 || ((char*)(*ptr) = getmem(nbytes)) == NULL)
	{
		kprintf("\nERR_2 sleepm");
		return(SYSERR);
	}
	*((word*)(*ptr) - 1) = i;
	slpMemtab[i].ptr = (char*)(*ptr);
	signal(meminsert_mutex);
	slpMemtab[i].secs = secs;
	return sleepmemt(i);
}
