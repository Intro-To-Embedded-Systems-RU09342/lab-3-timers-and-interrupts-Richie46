#include <msp430.h>

static volatile int counter = 0;            //global variable to count

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;                          //set to output
  P1OUT |= BIT1;                          //set to output
  P1DIR &= ~BIT1;                         //change to input
  P1REN |= BIT1;                          //enable resistor
  P1IES &= ~BIT1;                         //Have flag set on low to high
  P1IE |= BIT1;                           //enable interrupts for Pin 1.1
  P1IFG &= ~BIT1;                         //Clear Pin 1.1 flag so no immediate interrupt


  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  TA0CCTL0 = CCIE;                          // TA0CCR0 interrupt enabled
  TA0CCR0 = 3277;                           // TA0 set the max value to count to
  TA0CTL = TASSEL__ACLK | MC__UP;           // ACLK, up mode
  TA0CTL |= TACLR;                          // clear

  TA1CCTL0 |= CCIE;                      // TA1CCR0 interrupt enabled
  TA1CTL = TASSEL__ACLK | MC__STOP;      //config second timer
  TA1CCR0 = 512;                         //Set value of TA1CCR0 for comparison
  TA1CTL |= ID_3;                        //Divide clock by 8
  TA1EX0 |= TAIDEX_7;                    //Divide clock further by 8 to 512 hz
  TA1CTL |= TACLR;                       //clear

  __enable_interrupt();

  for(;;)
  {
  while(!(P1IN & BIT1))                 //if button is pressed
          {
              if(TA1R == 0)             //if value in timer1 is 0
              {
                  TA1CTL |= TASSEL_1 + MC__UP;    //Pick ACLK & Set Counter to Up Mode
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
__interrupt void Timer1_A0_ISR (void)       //timer1 interrupt
{
 counter++;                             //enable a counter
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)           //button interrupt
{
  TA1CTL &= ~TASSEL_1;                  //disable TA1
  TA0CCR0 = (counter)*3277;             //multiply the slowed down counter
  TA1CTL |= TACLR;                      //Clear Timer A1
  P1IFG &= ~BIT1;                       //Clear Pin 1.1 flag
}
