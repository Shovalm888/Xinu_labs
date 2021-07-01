/* Find if selected number is prime or not by N processes */

#include <conf.h>
#include <kernel.h>

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)
#define MAX_PROCS 30

unsigned int number_of_processes, unsuccessful_processes = 0;;
int  divider_was_found = 0;
unsigned long int number_under_check;
char buffer[128];

/*------------------------------------------------------------------------
 *  xmain  create N processes that checks if the selected number is prime or not
 *------------------------------------------------------------------------
 */
xmain()
{
	int	check_if_prime();
	int mutex, process_done, i, proc, max_processes, wait_for_procs_creation;
	unsigned long int initial_number;

	// Lock the processes we create from running until we will finish create them all 
	//because we have to know how many successful processes we got for the increment variable
	wait_for_procs_creation = screate(0);
	mutex = screate(1);
	process_done = screate(0);


	do {
		printf("\nEnter unsigned long integer (bigger then 0) : ");
		scanf("%lu", &number_under_check);
	} while (number_under_check <= 0);


	/*
	Max nuber of processes should be between 1 to :
	3 + 2(max - 1) <= number_under_check / 2
	6 + 4max - 4 <= number_under_check
	4max <= number_under_check - 2
	max <= (number_under_check - 2) / 4
	*/

	if (number_under_check == 1 || number_under_check == 2)
		goto end;

	if (number_under_check % 2 == 0) {
		printf("2 | %lu\t=>\t%lu  isn't prime!\n", number_under_check, number_under_check);
		return 0;
	}

	max_processes = MIN(MAX((unsigned int)((number_under_check - 2) / 4), 1), MAX_PROCS);
	do {
		printf("\nEnter number of processes (between 1 to %u): ", max_processes);
		scanf("%u", &number_of_processes);
		printf("\n");
	} while (number_of_processes > max_processes || number_of_processes <= 0);


	for (i = 0; i < number_of_processes; i++) {
		// Creating 'number_of_processes' processes:
		proc = create(check_if_prime, INITSTK, INITPRIO, "process", 4, i + 1, mutex, process_done, wait_for_procs_creation);
		if (proc >= 0) {
			if (!(resume(proc))) {

				unsuccessful_processes++;
				printf("*** Error resuming process number %d !!!***\n", i + 1);
			}
		}
		else {
			unsuccessful_processes++;
			printf("***Error creating process number %d !!!***\n", i + 1);
		}
	}

	printf("End to create process\n");

	// If processes weren't created well:
	if ((number_of_processes - unsuccessful_processes) == 0) {
		printf("\n***Number of successful processes is 0 !!!***\n");
		return -1;
	}
	// Unlock the processes we created
	for (i = 0; i < (number_of_processes - unsuccessful_processes) ; i++)
		signal(wait_for_procs_creation);

	// Wait just for the processes that created well:
	for (i = 0; i < (number_of_processes - unsuccessful_processes); i++)
		wait(process_done);

	if (divider_was_found) {
		if (printf("%s", buffer) <= 0) {
			printf("\n***Can't print the buffer!***\n");
		}
		return 0;
	}

end:
	printf("%lu is PRIME!\n", number_under_check);
	return 0;
}

/*------------------------------------------------------------------------
 *  check_if_prime  --  if number isn't prime print the range of the number we checked, otherwise just signal mutex
 *------------------------------------------------------------------------
 */
check_if_prime(serial_number, mutex, process_done, wait_for_procs_creation)
int serial_number, mutex, process_done, wait_for_procs_creation;
{
	unsigned int i;
	unsigned long int curr_number, inc, initial_number;
	// Wait for the main process to finish creating all the processes
	wait(wait_for_procs_creation);
	// Successful processes x 2
	inc = (number_of_processes - unsuccessful_processes) * 2;

	initial_number = serial_number - 1;
	initial_number = 3 + 2 * initial_number;
	curr_number = initial_number;

	while (curr_number <= (number_under_check / 2)) {

		// ----------------- Critical section starts here ----------------- 
		wait(mutex);

		if (divider_was_found) {

		end_the_search:
			printf("Process with pid %d started with %lu incremented by %lu. Tested up to %lu\n",
				getpid(), initial_number, inc, curr_number > initial_number ? curr_number - inc : curr_number);
			signal(process_done);
			signal(mutex);
			return 0;
		}

		// Check if curr_number divide number_under_check:
		else if (number_under_check % curr_number == 0) {

			divider_was_found = 1;

			if (sprintf(buffer, "\n%lu is NOT prime\n%lu / %lu = %lu\nWinner pid = %d, winner start = %lu\n",
				number_under_check, number_under_check,
				curr_number, number_under_check / curr_number, getpid(), initial_number) <= 0) {

				printf("\n***Copy string to buffer failed!***\n");
			}
			curr_number += inc;
			goto end_the_search;
		}
		// ----------------- Critical section ends here ----------------- 
		signal(mutex);
		curr_number += inc;
	}

	// When we didn't find divider we signal process_done to let xmain process know that we done checking in this process
	signal(process_done);

	return 0;
}

