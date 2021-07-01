/* xwait.c - wait for children/s */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include "lab6_h.h"

SYSCALL xwait()
{
	waitingPids[currpid] = 1;

	if (sons[currpid] > 0)
		wait(semphs[currpid]);
	else
		return 0;
	
	return 1;
}

SYSCALL xwaitAll()
{
	// father[NPROC], sons[NPROC], waitingPids[NPROC], semphs[NPROC];
	while (xwait() > 0);

	return 1;
}