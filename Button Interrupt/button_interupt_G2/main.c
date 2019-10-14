#include <msp430.h> 

/**
 * main.c
 */
static volatile int enable_blink = 0;;      //make volatile variable
                                            //prevent optimization

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1REN |= BIT3;                          //enable resistor
    P1DIR |= BIT0;                          // set P1.0 to output
    P1OUT |= BIT3;                          //set P1.4 to input
    P1IE |= BIT3;                           //enable interrupt
    P1IES |= BIT3;                          // enable interrupt at rising edge
    P1IFG &= ~BIT3;                         //clear interrupt flag
    __enable_interrupt();

    while(1)
    {
        if (enable_blink == 1)              //if button is pressed
        {
        P1OUT ^= BIT0;                      //blink LED
        __delay_cycles(100000);
        }
    }

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)           //button interrupt
{
    enable_blink ^= 1;                   //enable blink
    P1IFG &= ~BIT3;                      // P1.1 IFG cleared
}

