/* $Id: powerbutton.c,v 1.3 2011/06/23 07:35:36 wschaub Exp wschaub $  */
/* read /dev/input/event0 forever until the power key is pressed and then initiate a clean poweroff */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <linux/input.h>
/* struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
}; */

int main(int argc, char **argv) {
int opt,foreground; 
foreground = 0;

while ((opt = getopt(argc, argv, "f")) != -1) {
    switch (opt) {
           case 'f':
               foreground = 1;
               break;
           default: /* '?' */
               fprintf(stderr, "Usage: %s [ -f ]\n",
                       argv[0]);
               exit(EXIT_FAILURE);
           }
}

if(!foreground) {
    daemon(0,0);
}

struct input_event event; //Each read of event0 gives us an input_event struct
FILE *fp;
fp = fopen("/dev/input/event0","r"); //XXX! we should check for errors here...
if(fp == NULL)
{
    perror("/dev/input/event0");
    abort();
}

//Loop forever reading events and do something only when the power key is pressed.
    while(1) 
    {
        fread(&event,sizeof(event),1,fp);
        //printf("type = %d\n",event.type);
        //printf("code = %d\n",event.code);
        if(event.type == EV_KEY && event.code == KEY_POWER) {
            printf("power key pressed\n");
            system("/sbin/poweroff");
            return 0;
        }
    }

}


