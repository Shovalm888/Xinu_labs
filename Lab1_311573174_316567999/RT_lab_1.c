#include<stdio.h>
#include<dos.h>

typedef unsigned long int *FUN_PTR;

// GLOBALS:
char save, *watch_str;
FUN_PTR savep, watch_ptr;



void interrupt (*Int3Save)(void);

void set_watch(char print_str[], unsigned long int *watchpt){

	watch_ptr = watchpt;
	watch_str = print_str;
	
}

void interrupt int3_Handler(void){
	
	printf(watch_str, *watch_ptr);
	printf("\nPress any key to continue:\n");
	*((char*)savep) = save;

	asm{
		STI
		MOV AH,0
		INT 16h
		mov bx,bp
		add bx,18
		dec word ptr ss:[bx]
	}	
}

unsigned long int div(unsigned long int num, unsigned long int denom) {return (num / denom);}

void take_int3(void interrupt(*int3_Handler)(void)){
	
	 Int3Save = getvect(3);
	 setvect(3, int3_Handler);

}



void set_breakpoint(FUN_PTR brkpt){
	save = *((char*)brkpt);
	savep = brkpt;
	*((char*)brkpt) = 0xcc;	
}

int compute_factors(unsigned long int n, unsigned long int factors[]){
	unsigned long int i;
	int j = 0;
	take_int3(int3_Handler);
	for(i = 2; i <= n ; i++)
		while((n % i) == 0){
			set_breakpoint(div);
			set_watch("debugger message: n = %lu\n", &n);
			
			n = div(n, i);
			factors[j++] = i;
		}
	setvect(3, Int3Save);
	return j;
}

void main()
{
	unsigned long int n, array[32];
	int i , j;
	printf("\n Enter n:\n");
	scanf("%lu", &n);
	printf("n = %lu\n", n);
	j = compute_factors(n, array);
	printf("factors:\n");
	for (i = 0 ; i < j ; i++)
		printf(" %lu ", array[i]);    
}