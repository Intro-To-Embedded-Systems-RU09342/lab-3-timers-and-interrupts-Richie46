#include <msp430.h> 

/**
 * main.c
 */
static volatile int enable_blink = 0;;      //make volatile variable
                                            //prevent optimization

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1REN |= BIT1;                          //enable resistor
    P1DIR |= BIT0;                          // set P1.0 to output
    P1OUT |= BIT1;                          //set P1.1 to input
    P1IE |= BIT1;                           //enable interrupt
    P1IES |= BIT1;                          //enable interrupt on rising edge
    P1IFG &= ~BIT1;                         //clear IFG
    __enable_interrupt();

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

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
    P1IFG &= ~BIT1;                      // P1.1 IFG cleared
}
