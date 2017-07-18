#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void infinite_loop(){
    for (;;){
    }
}

int main () {
    pid_t childPid;

    childPid = fork();

    if (childPid >= 0) { //success forking
	if (childPid == 0) //child here
	    infinite_loop();
	
	if (childPid > 0){ //parent here
	    printf ("child process forked with pid %i\n", childPid);
	    printf ("parent exits without waiting\n");
	    exit (1);
	}
   }
    else { //everything went tits up
	printf("TERMINATE! TERMINATE!!\n");
    }
    return 0;
}    


