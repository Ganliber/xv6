#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


/*
* your sleep should pause for a user-specified number of ticks.
* A tick is a notion of time defined by the xv6 kernel, namely 
* the time between two interrupts from the timer chip.
*/

int
main(int argc, char *argv[])
{
    if(argc < 2){
        //If the user forgets to pass an argument, sleep should print an error message.
        fprintf(2, "usage: sleep [number]\n");
        exit(1);
    }
    if (argc == 2)
    {
        /* code */
        int interval = atoi(argv[1]);
        printf("(nothing happens for a little while)\n");
        sleep(interval);
    } else {
        fprintf(2, "Too many arguments!\n");
        exit(1);
    }
    exit(0);
}