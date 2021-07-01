/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*--------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *--------------------------------------------------------------------
 */
SYSCALL	freep(block)
char *block;
{
	return (freemem(block, *((word*)(block - 4))));
}
