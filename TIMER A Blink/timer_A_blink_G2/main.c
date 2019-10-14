#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;                        //set P1.0 to output
  P1OUT |= BIT0;                        //turn on LED
  P1DIR |= BIT6;                        //set P1.6 to output
  P1OUT |= BIT6;                        //turn on LEd

  TA0CCTL0 = CCIE;                          // TA0CCR0 interrupt enabled
  TA0CCR0 = 10000;                          // TA0 set the max value to count to
  TA0CTL = TASSEL_1 | MC_1;           // ACLK, up mode

  TA1CCTL0 = CCIE;                          // TA1CCR0 interrupt enabled
  TA1CCR0 = 50000;                          // TA1 set the max value to count to
  TA1CTL = TASSEL_1 | MC_1;           // ACLK, up mode


  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)       //timer0 interrupt
{
  P1OUT ^= BIT0;                        //flash LED1
}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)       //timer1 interrupt
{
  P1OUT ^= BIT6;                        //flash LED2
}

