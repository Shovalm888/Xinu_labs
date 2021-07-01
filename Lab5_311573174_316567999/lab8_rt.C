#include <stdio.h>
#include<stdlib.h>
#include<dos.h>

#define INPUT_SIZE 200
#define ON (1)
#define OFF (0)



void interrupt(*int112Save)(void);
void interrupt(*int9Save)(void);
int num_of_int8[INPUT_SIZE];
int sc = -1, prev_sc = -1;
int arr_size = 1;


void ChangeSpeaker(int status)
{
    int portval;
    //   portval = inportb( 0x61 );

    portval = 0;
    asm{
         PUSH AX
         IN AL,61h
         MOV byte ptr portval,AL
         POP AX
    }

        if (status == ON)
            portval |= 0x03;
        else
            portval &= ~0x03;
    // outportb( 0x61, portval );
    asm{
      PUSH AX
      MOV AX,portval
      OUT 61h,AL
      POP AX
    } // asm

}

void Sound(int latch)
{

    ChangeSpeaker(ON);

    //        outportb( 0x43, 0xB6 );
    asm{
      PUSH AX
      MOV AL,0B6h // write to counter-2
      OUT 43h,AL
      POP AX
    } // asm


 //       outportb( 0x42, divisor & 0xFF ) ;
        asm{
          PUSH AX
          MOV AX,latch
          AND AX,0FFh
          OUT 42h,AL
          POP AX
    } // asm


 //        outportb( 0x42, divisor >> 8 ) ;

        asm{
          PUSH AX
          MOV AX,latch
          MOV AL,AH
          OUT 42h,AL
          POP AX
    } // asm

}

void NoSound(void)
{
    ChangeSpeaker(OFF);
} /*--NoSound( )------*/


void interrupt newint112(void) {

    int is_periodic = 0;

    asm{
         PUSH AX
         PUSH BX
         xor ax, ax
         IN AL,70h
         MOV bl,al
         MOV AL,0Ch
         OUT 70h,AL
         MOV AL,8Ch
         OUT 70h,AL
         IN AL,71h
         and al, 01000000b
         mov is_periodic, ax
         MOV al,bl
         OUT 70h,AL
         MOV AL,20h
         OUT 0A0h,AL
         OUT 020h,AL
         POP BX
         POP AX
        
        } // asm */
        
        if (is_periodic =! 0 && arr_size <= INPUT_SIZE)
            num_of_int8[arr_size - 1]++;


}/* end 70H */


void interrupt newint9(void)
{
    int msb, i;

    if (arr_size < INPUT_SIZE)
    {

        asm{
            push ax
            xor ax, ax
            in al, 60h
            mov sc, ax
            pop ax
        }

        msb = sc >> 7;
        sc = sc & 127;
        //printf("\nsc : %u\tmsb : %u", sc, msb);

        if (msb == 0 && prev_sc != sc && sc != 1)
        {
            prev_sc = sc;
            Sound(num_of_int8[arr_size - 1]);
            arr_size++;
        }

    }

    asm{
        PUSHF
        CALL DWORD PTR int9Save
    }
}

int compare_function(const void* a, const void* b) {
    unsigned int* x = (unsigned int*)a;
    unsigned int* y = (unsigned int*)b;
    return *x - *y;
}


void main(void) {
    int i, sum;
    char old_master_mask, old_slave_mask, old_A_mask, old_B_mask;
    

    // Global array initiation

    for (i = 0; i < INPUT_SIZE; i++)
        num_of_int8[i] = 0;


    // Activating the RTC and preserving all the mask we change


    asm{
        cli
        push ax
        push bx
        xor ax, ax
        // turn on the slave
        in al, 21h
        mov old_master_mask, al
        and al, 11111011b
        out 21h, al
        // turn on the RTC
        in al, 0a1h
        mov old_slave_mask, al
        and al, 11111110b
        out 0a1h, al
        // determine clock frequency
        mov al, 0ah
        out 70h, al
        mov al, 08ah
        out 70h, al
        in al, 71h
        mov old_A_mask, al
        mov al, 00100110b
        out 71h, al
        in al, 71h
        // activate interrupt type
        mov al, 0bh
        out 70h, al
        mov al, 08bh
        out 70h, al
        in al, 71h
        mov old_B_mask, al
        or al, 01000000b // only periodic interrupt
        out 71h, al
        in al, 71h
        // reading from C and D due to the protocol
        mov al, 0ch
        out 70h, al
        mov al, 08ch
        out 70h, al
        in al , 71h
        mov al , 0dh
        out 70h, al 
        in al , 71h
        pop bx
        pop ax
        sti

    } // asm


    // Replace the code that activated by IRQ8 & IRQ 1 

    int112Save = getvect(0x70);
    setvect(0x70, newint112);
    int9Save = getvect(9);
    setvect(9, newint9);
    flushall();

    // Take input from user

    printf("\n***To exit press ESC***\n");
    while (arr_size <= INPUT_SIZE && sc != 1);


    // Restore the code that activated by IRQ8 & IRQ 1 

    setvect(0x70, int112Save);
    setvect(9, int9Save);
    NoSound();


    // Restoring port 70h (A & B registers), slave and master values::


    asm{
    cli
    push ax
    push bx
    xor ax, ax
        // return slave's original mask
        and al, old_master_mask
        out 21h, al
        // return RTC's original mask
        and al, old_slave_mask
        out 0a1h, al
        // return clock's original frequency
        mov al, 0ah
        out 70h, al
        mov al, 08ah
        out 70h, al
        mov al, old_A_mask
        out 71h, al
        in al, 71h
        // return register B original mask
        mov al, 0bh
        out 70h, al
        mov al, 08bh
        out 70h, al
        or al, old_B_mask 
        out 71h, al
        in al, 71h
        // reading from C and D due to the protocol
        mov al, 0ch
        out 70h, al
        mov al, 08ch
        out 70h, al
        in al , 71h
        mov al , 0dh
        out 70h, al
        in al , 71h
        pop bx
        pop ax
        sti

    } // asm


	//remove first click time
	num_of_int8[0]=num_of_int8[arr_size-1];
	arr_size--;

    // print the requested values::

	if(arr_size>0)
	{
		printf("\nTimed\n");
		for (sum = 0, i = 0; i < arr_size; i++)
		{
			printf("%u, ", num_of_int8[i]);
			sum += num_of_int8[i];
		}
		printf("\nSorted\n");
		qsort(num_of_int8, arr_size, sizeof(int), compare_function);
		for (i = 0; i < arr_size; i++)
			printf("%u, ", num_of_int8[i]);
		printf("\nMin_time : %u / 1069 sec", num_of_int8[0]);
		printf("\nMax_time : %u / 1069 sec", num_of_int8[arr_size - 1]);
		printf("\nMed_time : %u / 1069 sec", num_of_int8[arr_size / 2]);
		printf("\nTotal_time : %u / 1069 sec", sum);
	}
	else
		printf("No clicks");
    return;

}

