#include <conf.h>
#include <kernel.h>
//**LAB_6
#include "lab6_h.h"
#include <proc.h>
#include <mem.h>


xmain()
{
    int Inc();
    int pid, i;
    char* ptr, *ptm, *ptc, * pth;

    // ############### Test - 1 start - xforkSonFirst - ############### //

    //printf("\nFather pid : %d , before forking", getpid());

    //xforkSonFirst();

    //printf("\nExpected son's pid before father, pid : %d", getpid());
  
    // ############### Test - 1 end ############### //

    // ############### Test - 2 start - xwait - ############### //

    //printf("\nFather pid : %d , before forking", getpid());

    //if (xfork() == 0)
    //{
    //    printf("\npid : %d , sleep for 5 sec", getpid());
    //    sleep(5);
    //}
    //else if (!xfork())
    //{
    //    printf("\npid : %d , sleep for 3 sec", getpid());
    //    sleep(3);
    //}

    //xwait();
    //
    //printf("\nExpected son -> father -> son, pid : %d", getpid());

    // ############### Test - 2 end ############### //

    // ############### Test - 3 start - xwaitAll - ############### //

    //printf("\nFather pid : %d , before forking", getpid());

    //if (xfork() == 0)
    //{
    //    printf("\npid : %d , sleep for 5 sec", getpid());
    //    sleep(5);
    //}
    //else if (!xfork())
    //{
    //    printf("\npid : %d , sleep for 3 sec", getpid());
    //    sleep(3);
    //}

    //xwaitAll();
    //
    //printf("\nExpected son -> son -> father, pid : %d", getpid());

    // ############### Test - 3 end ############### //

    // ############### Test - 4 start - getmemForTime - ############### //

    //getmemForTime(80, 3, &ptc);
    //getmemForTime(70, 4, &ptr);
    //getmemForTime(60, 5, &pth);
    //getmemForTime(50, 6, &ptm);
    //printf("\nmem_1 expected ~ 80, result : %u", *(word*)(ptc - 4));
    //printf("\nmem_2 expected ~ 70, result : %u", *(word*)(ptr - 4));
    //printf("\nmem_3 expected ~ 60, result : %u", *(word*)(pth - 4));
    //printf("\nmem_4 expected ~ 50, result : %u", *(word*)(ptm - 4));
    //printf("\nslpMemtab.ptr[0] expected not NULL, result : %d", slpMemtab[0].ptr);
    //printf("\nslpMemtab.ptr[1] expected not NULL, result : %d", slpMemtab[1].ptr);
    //printf("\nslpMemtab.ptr[2] expected not NULL, result : %d", slpMemtab[2].ptr);
    //printf("\nslpMemtab.ptr[3] expected not NULL, result : %d", slpMemtab[3].ptr);
    //freemem(ptc, 80);
    //freemem(pth, 20);
    //printf("\nslpMemtab.ptr[0] expected NULL, result : %d", slpMemtab[0].ptr);
    //printf("\nslpMemtab.ptr[1] expected not NULL, result : %d", slpMemtab[1].ptr);
    //printf("\nslpMemtab.ptr[2] expected NULL, result : %d", slpMemtab[2].ptr);
    //printf("\nslpMemtab.ptr[3] expected not NULL, result : %d", slpMemtab[3].ptr);
    //sleep(8);
    //for (i = 0; i < 4; i++)
    //    printf("\nslpMemtab.ptr[%d] expected NULL, result : %d",i, slpMemtab[1].ptr);

    // ############### Test - 4 end ############### //
    
    // ############### Test - 5 start - freep - ############### //

    //getmemForTime(80, 3, &ptc);
    //ptr = getmem(100);

    //printf("\nexpected SYSERR : %d", freep(pth));
    //printf("\nexpected OK : %d", freep(ptc));
    //printf("\nexpected OK : %d", freep(ptr));

    // ############### Test - 5 end ############### //
}

/*------------------------------------------------------------------------
 *  Inc  --  Increment counter via pointer
 *------------------------------------------------------------------------
 */
Inc()
{
    int i = 3;
    if (xforkSonFirst())
        xforkSonFirst();
    
    xwaitAll();
    while (i-- > 0) 
    {
        printf("%s ", proctab[getpid()].pname);
        sleep(1);
    }
    
}  /* Inc */


