/*
 * Simple zombie tester
 *
 * Created by mike.merinoff
 * 2014
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void infinite_loop(){
    for (;;)
	asm ("nop");
    
}

int main () {
    pid_t childPid;

    childPid = fork();
    if (childPid >= 0) { //success forking
	if (childPid == 0) //child here
	    exit (-1);
	
	if (childPid > 0){ //parent here
	    printf ("child process forked with pid %i\n", childPid);
	    infinite_loop();
	}
   }
    else { 
	printf("TERMINATE! TERMINATE!!\n");
    }
    return 0;
}    


