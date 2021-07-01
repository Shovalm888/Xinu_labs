#include <stdio.h>
#include<stdlib.h>
#include<dos.h>



#define INPUT_SIZE 200
#define ON (1)
#define OFF (0)

void interrupt(*int8Save)(void);
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

void Sound(int latchVal)
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
		  MOV AX,latchVal
		  AND AX,0FFh
		  OUT 42h,AL
		  POP AX
	} // asm


 //        outportb( 0x42, divisor >> 8 ) ;

		asm{
		  PUSH AX
		  MOV AX,latchVal
		  MOV AL,AH
		  OUT 42h,AL
		  POP AX
	} // asm

}

void NoSound(void)
{
	ChangeSpeaker(OFF);
} /*--NoSound( )------*/

void interrupt newint8(void)
{
	
	if (arr_size <= INPUT_SIZE)
		num_of_int8[arr_size - 1]++;
	asm{
		PUSHF
		CALL DWORD PTR int8Save
	}
}

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
	int* x = (int*)a;
	int* y = (int*)b;
	return *x - *y;
}

int main()
{
	int i, sum, oldLatchSave;


	// Global array initiation


	for (i = 0; i < INPUT_SIZE; i++)
		num_of_int8[i] = 0;



	// Replace the latch and preserve the original 


	asm{
		cli
		XOR AX,AX
		IN  AL, 43H
		mov bl, al
		IN AL, 43H
		mov bh, al
		mov oldLatchSave, bx
		MOV AL,36h
		OUT 43H,AL
		mov bx, 700
		mov al, bl
		out 40h, al
		mov al, bh
		out 40h, al
		sti
	}
	


	// Replace the code that activated by IRQ0 & IRQ 1 


	int8Save = getvect(8);
	setvect(8, newint8);
	int9Save = getvect(9);
	setvect(9, newint9);
	flushall();


	// Take input from user


	printf("\n***To exit press ESC***\n");
	while (arr_size <= INPUT_SIZE && sc != 1);


	// Restore the original latch

	asm{
		cli
		MOV AL, 36h
		OUT 43H, AL
		mov bx, oldLatchSave
		mov al, bl
		out 40h, al
		mov al, bh
		out 40h, al
		sti
	}



	// Restore the code that activated by IRQ0 & IRQ 1 

	setvect(8, int8Save);
	setvect(9, int9Save);
	NoSound();


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