#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]){

	if(argc < 2){
		fprintf(2,"too few argument for sleep \"system\" call!\n");
	}

	unsigned int interval = atoi(argv[1]);
	// interval *= 1000; // msec to sec
	sleep(interval);
	exit(0);
}