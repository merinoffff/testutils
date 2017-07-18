/*
 * Zombie tester with signaling example 
 * 
 * Created by mike.merinoff
 * 2014, 2017
 *
 * TODO:
 * add fetching child pid in signal handler
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
// terminal features
#include <termios.h>
#include <fcntl.h>


pid_t childPid;


void panic_and_run (int signum){
    if (signum == SIGCHLD){
	printf ("(p) ++ child process forked with pid %i ++\n", childPid);
	printf ("(p) ++ please press 'r' to rip the child ++\n");
        sleep (2);
    }
    else
	printf ("(p) ++ %i signal is received ++\n", signum);
}


void rip_child(){

    int wstatus;
    wait (&wstatus);
    printf("(p) ++ child %i ripped ++\n", childPid);
    return;

}


void quit (){
    printf("(p) ++ quitting ++\n");
    exit (0);
}

void infinite_loop(){
    char input = '\0';
    int kill_child = 0;
    for (;;){
        input=getchar();
        if (!strncmp(&input, "q", 1))
            quit();
        if (kill_child == 0 && !strncmp(&input, "r", 1) ){
            kill_child = 1;
            rip_child();
        }
        else
            asm ("nop");
    }
    
}

void setup_terminal ()
{
    // hide input from stdin
    struct termios tattr;
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);	/* Clear ICANON and ECHO. */
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);

    // set STDIN to nonblocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

}
int main () {
    setup_terminal();

    signal (SIGCHLD, panic_and_run);

    childPid = fork();
    if (childPid >= 0) { //success forking
	if (childPid == 0) //child here
        {
            //sleep (5);
            printf("(c) == ready to be ripped ==\n");
	    exit (-1);
        }
	if (childPid > 0) //parent here
	    infinite_loop();
	
   }
    else { //everything went tits up
	printf("TERMINATE! TERMINATE!!\n");
    }
    return 0;
}    


