
#define NUM_OF_SLPMEMS 50

#define NEWNQENT           NQENT + NUM_OF_SLPMEMS + 2 /*  ready & sleep & sleep memo    */

typedef struct slpMem 
{
	char* ptr;
	int secs;
} slpMem;


//extern SYSCALL getmemForTime(nbytes, secs, ptr), sleepmemt(ind);
extern slpMem slpMemtab[NUM_OF_SLPMEMS];
extern int father[NPROC], sons[NPROC], waitingPids[NPROC], semphs[NPROC];
extern int * slp_top, meminsert_mutex, slp_head, slp_tail, slp_empty;
extern int is_timemem;
extern int newnqent;

