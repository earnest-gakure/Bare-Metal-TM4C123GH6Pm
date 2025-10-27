//HAS NOT worked
#include "TM4C123.h"                    // Device header
/*
using pwm to control a LED
PWM 0 -> chanel 1 (pB7)



*/
int main()
{
	
	
	/*clock setting for PWM and GPIO port */
	SYSCTL->RCGC0 = (1<<1) ;/*enable clock for pwm 0 */
	//SYSCTL->RCGCGPIO |= 0x02 ;/*enable clock for pwm gpio B*/
	SYSCTL->RCGCGPIO |= (1<<5);/*enable clock for GpioF */
  SYSCTL->RCC  &= (~(1<<20));/*configure system clock module */
  
   //pF2
	GPIOF->AFSEL |= (1<<2);
	GPIOF->PCTL = 0x00000500;
	GPIOF->DEN |= (1<<2);
	//GPIOF->DIR |= (1<<2);
	
	/*seeting PB7 pin for M0PWM1 channel output pin*/
//	GPIOB->AFSEL |= (1<<7);       /*pB7 alternate function */
//	GPIOB->PCTL = 0x04000000 ; /*PB7 PWM output pin */
//	GPIOB->DEN |= (1<<7) ;        /*PB7 set to digital */
//	
//   /*PWM0 channel 1 setting */
//	PWM0->_0_CTL &= (~(1<<0));   /*disable generator 0 block  */
//	PWM0->_0_CTL &= (~(1<<1)) ; /* select down count mode for counter 1*/
//	PWM0->_0_LOAD = 1600 ;      /* set load value  for for 1KH (16MHz/1600)*/
//  PWM0->_0_CMPB = 800 ;       /* set duty cycle to 50% by loading of 16000 to PWM0CMPA*/ 
//	PWM0->_0_GENB |= (1<<2) | (1<<3) | (1<<7 );/*set PWM output when counter reloaded and clear when matches PWMCMPA */
//	PWM0->_0_CTL |= (1<<0);/* Enable Genenrator 1 block */ /*when load value is reached make pin high */
//	PWM0->ENABLE |= (1<<1) ;/*Enable PWM channel 1 output */


  PWM1->_3_CTL &= (~(1<<0));   /*disable generator 0 block  */
	PWM1->_3_CTL &= (~(1<<1)) ; /* select down count mode for counter 1*/
	PWM1->_3_LOAD = 1600 ;      /* set load value  for for 1KH (16MHz/1600)*/
  PWM1->_3_CMPA = 800 ;       /* set duty cycle to 50% by loading of 16000 to PWM0CMPA*/ 
	PWM1->_3_GENA |= (1<<2) | (1<<3) | (1<<7 );/*set PWM output when counter reloaded and clear when matches PWMCMPA */
	PWM1->_3_CTL |= (1<<0);/* Enable Genenrator 1 block */ /*when load value is reached make pin high */
	PWM1->ENABLE |= (1<<6) ;/*Enable PWM channel 1 output */


   while (1)
	 {
		 
		 
	 }
	 
}
