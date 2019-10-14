#include <msp430.h>

static volatile int counter = 0;            //global variable to count

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;                          //set to output
  P1OUT |= BIT3;                          //set to output
  P1DIR &= ~BIT3;                         //change to input
  P1REN |= BIT3;                          //enable resistor
  P1IES &= ~BIT3;                         //Have flag set on low to high
  P1IE |= BIT3;                           //enable interrupts for Pin 1.3
  P1IFG &= ~BIT3;                         //Clear Pin 1.3 flag so no immediate interrupt

  TA0CCTL0 = CCIE;                          // TA0CCR0 interrupt enabled
  TA0CCR0 = 3277;                           // TA0 set the max value to count to
  TA0CTL = TASSEL_1 | MC_1;                 // ACLK, up mode
  TA0CTL |= TACLR;                          // clear

  TA1CCTL0 |= CCIE;                      // TA1CCR0 interrupt enabled
  TA1CTL = TASSEL_1 | MC_0;              //ACLK, stop mode
  TA1CCR0 = 4096;                        //Set value of TA1CCR0 for comparison
  TA1CTL |= ID_3;                        //Divide clock by 8
  TA1CTL |= TACLR;                       //clear

  __enable_interrupt();

  for(;;)
  {
  while(!(P1IN & BIT6))             //when button is pressed
          {
              if(TA1R == 0)          //if value of timer1 is 0
              {
                  TA1CTL |= TASSEL_1 + MC_1;    //Pick ACLK & Set Counter to Up Mode
              }
          }
  }
}

// Timer0_A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)       //timer0 interrupt
{
  P1OUT ^= BIT0;                        //flash LED
}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR (void)       //timer0 interrupt
{
 counter++;             //enable a counter
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)           //button interrupt
{
  TA1CTL &= ~TASSEL_1;                  //disable TA1
  TA0CCR0 = counter*3277;               //multiply the slowed down counter
  TA1CTL |= TACLR;                      //Clear Timer A1
  P1IFG &= ~BIT3;                       //Clear Pin 1.3 flag
}
