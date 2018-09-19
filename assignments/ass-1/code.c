#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "Global string";
const int constant = 21;

int main() {
	int pid = getpid();
foo:
	printf("process id: %d\n", pid);
	printf("global string: %p\n", &global); // start address of global array
	printf("global constant: %p\n", &constant);
	printf("the code: %p\n", &&foo); // && gets the address of a label. If & is used, it is treated like a variable/lvalue

	printf("\n\n /proc/%d/maps \n\n", pid);
	char command[50];
	sprintf(command, "cat /proc/%d/maps", pid);
	system(command); // performs shell command
	return 0;
}
