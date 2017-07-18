/*
 * Advanced zombie example
 * Could be used to test e.g. monitoring systems
 *
 * Created by mike.merinoff
 * 2017
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


int dead_alive = 0;

void rip_child(){
    int wstatus;
    pid_t pid = wait (&wstatus);
    if (pid > 0) {
        printf("(p) -- child %i ripped --\n", pid);
        dead_alive--; 
    }
}


void quit (){
    printf("(p) \\\\ quitting //\n");
    exit (0);
}


void create_zombie () {
    pid_t childPid = fork();
    if (childPid >= 0) { //success forking
	if (childPid == 0) //child here
        {
            sleep (0.1);
            printf("(c) ++ %i ready to be ripped ++\n", getpid());
	    exit (-1);
        }
	if (childPid > 0){ //parent here
            dead_alive++;
	    //printf ("(p) ++ zombie forked; %i to rip ++\n", dead_alive);
        }
   }
    else { //everything went tits up
	printf("TERMINATE! TERMINATE!!\n");
    }
} 

 void infinite_loop(){
    char input = '\0';
    for (;;){
        input=getchar();
        switch (input){
            case 'c':
            case '+':
                    create_zombie();
                    break;
            case '-':
            case 'r':
                    if (dead_alive > 0)
                        rip_child();
                    else
                        printf("(p) ++ all dead, thank you very much ++\n");
                    break;
            case 'a':
                    while (dead_alive-- > 0) //probably broken logic here
                        rip_child();
                    printf("(p) ++ all dead, thank you very much ++\n");
                    break;
            case '?':
                    printf("(p) ?? 'r' to rip a zombie ??\n");
                    printf("(p) ?? 'c' to spawn a zombie ??\n");
                    printf("(p) ?? 'a' kill all zombies (doesn't yet work) ??\n");
                    printf("(p) ?? 'q' to quit ??\n");
                    printf("(p) ?? '?' see this help ??\n");
                    printf("(p) == %i active zombies ==\n", dead_alive);
                    break; 
            case 'q':
                    quit();
            }

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


int main (int argc, char ** argv) {

    setup_terminal();
    
    int init_zombies = 1;

    printf ("(p) ?? '?' to see help ??\n");

    if  (argc > 1)
        init_zombies = atoi (argv[1]);

    printf("(p) ++ creating %i zombies ++\n", init_zombies); 
    while (init_zombies-- > 0)
        create_zombie();

    // main loop
    infinite_loop();

    return 0;
}    


