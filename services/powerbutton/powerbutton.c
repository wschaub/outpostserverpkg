/* $Id: powerbutton.c,v 1.3 2011/06/23 07:35:36 wschaub Exp wschaub $  */
/* read /dev/input/event0 forever until the power key is pressed and then initiate a clean poweroff */
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

struct input_event event; //Each read of event0 gives us an input_event struct
FILE *fp;
fp = fopen("/dev/input/event0","r"); //XXX! we should check for errors here...

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

/*
 * $Log: powerbutton.c,v $
 * Revision 1.3  2011/06/23 07:35:36  wschaub
 * we now execute /sbin/poweroff on a power button press
 *
 * Revision 1.2  2011/06/23 07:24:40  wschaub
 * changed exit to return
 *
 * Revision 1.1  2011/06/23 07:20:28  wschaub
 * Initial revision
 *
 */
