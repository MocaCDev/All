#ifndef speaker
#define speaker
#include "util.h"
#include "pit.h"

void play_sound(uint32 freq)
{
    uint32 div  = 0;
    uint8 tmp  = 0;

    // Set the PIT to the desired frequency
    div = 1193180 / freq;
    PORT_OUT(0x43, 0xB6);
    PORT_OUT(0x42, (uint8)div);
    PORT_OUT(0x42, (uint8)div>>8);

    // Play sound
    tmp = PORT_IN(0x61);
    if(tmp != (tmp | 3))
        PORT_OUT(0x61, tmp|3);
}

void no_sound()
{
    uint8 tmp = PORT_IN(0x61) & 0xFC;
    PORT_OUT(0x61, tmp);
}

void beep()
{
    play_sound(1000);
    //sleep(10);
    no_sound();
}

#endif