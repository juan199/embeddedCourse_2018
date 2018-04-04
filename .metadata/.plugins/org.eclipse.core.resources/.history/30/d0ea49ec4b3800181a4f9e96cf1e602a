#include "msp.h"


/*
 * main.c
 */

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    P2->DIR = BIT0 | BIT1 | BIT2;
    P2->OUT = BIT0 | BIT1 | BIT2;

    int COUNTER_BLINK = 0;

    while(true){
        if (COUNTER_BLINK >=120000){
            P2-> OUT++;
            COUNTER_BLINK = 0;
        }
        COUNTER_BLINK++;
    }
}

// comments to explain what for, but how to
