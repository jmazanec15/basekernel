#include "library/syscalls.h"
#include "library/user-io.h"
#include "library/string.h"

/*
	This test shows how to properly use syscall_open_pipe. A pipe
	connects the output of one process to the input of another. Pipes are
	typically used for one way, interprocess comunnication.
*/

int main(int argc, char *argv[])
{
	/*
		Create the pipe, which is just a file descriptor, make it non blocking,
		and then fork a child process.
	*/
	printf("%d: Running pipe test!\n", syscall_process_self());
	int w = syscall_open_pipe();
	syscall_object_set_blocking(w, 0);
	int x = syscall_process_fork();

	/*
		Then, have the parent duplicate the pipe over its standard output. Now, all
		writes to file descriptor 1 will go to the pipe instead of to the screen.

		The child process then waits for "Testing" to be written to the pipe and
		once it does, the child process reads it and prints it out to standard
		output.
	*/
	if(x) {
		printf("%d: Writing...\n", syscall_process_self());
		syscall_object_dup(w, KNO_STDOUT);
		printf("Testing!");
		syscall_process_sleep(1000);
	} else {
		printf("%d: Reading...\n", syscall_process_self());
		int r;
		char buf[10];
		while(!(r = syscall_object_read(w, buf, 10))) {
			syscall_process_yield();
		}
		printf("%d: I read %d chars from my brother\n", syscall_process_self(), r);
		printf("%d: They are (%s)\n", syscall_process_self(), buf);
	}
	return 0;
}
