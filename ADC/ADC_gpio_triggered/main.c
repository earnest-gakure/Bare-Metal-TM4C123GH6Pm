/*configuring a gpio to trigger ADC conversion
    Timer trigers GPIO pin interrupt 
		               |
	  the interrupt sets the pin HIGH
		               |
		The pin triggers ADC conversion when it gets HIGH
		               |
		ADC result turn on an LED if its above 2048(1.65 v)
#include "TM4C123.h"                    // Device header
#include "TM4C123.h"                    // Device header
*/
#include "TM4C123.h"                    // Device header
#include <stdint.h>
#define  GREEN 0x08
#define  RED 0x02
#define  BLUE 0x04
//function prototypes
void timer_interrupt(void);
void TIMER0A_Handler(void);
void adc_init(void);
void LED_control(uint16_t adcValue);
void gpio_interrupt (void);
void adc_read(void);
volatile int adc_value ;
int main()
{
	
	SYSCTL->RCGCGPIO |= 0x20 ;
	GPIOF->DIR = 0x0E ;
 	GPIOF->DEN = 0x0E ;
	timer_interrupt();
	gpio_interrupt(); //calling gpio interrupt function
	adc_init(); //adc initialization
	__enable_irq();
	
	while (1)
	{
		
	}
	
}

//configure timer as interrupt trigger
void timer_interrupt(void)
{
	
	SYSCTL->RCGCTIMER |= 0x01;    //clock for timer 0
	TIMER0->CTL &= ~0x01;         //disable timer
	TIMER0->CFG = 0x04 ;       //16 bit timer
	TIMER0->TAMR = 0x02;      //periodic mode
	TIMER0->TAPR = 250 ;       //prescaler 16MHz/250 = 64000Hz
	TIMER0->TAILR = 128000;   //load  time = 64000/64kHz	TIMER0->ICR = 0x01 ;    //clear interrupts
	TIMER0->IMR = 0x01 ;       //timer A timeout interrupt mask
	TIMER0->CTL |= 0x01;     //enable timer
	NVIC->ISER[0]= (1U<<19);         //Enable IRQ23 (D19 of ISER[0] )
}
void TIMER0A_Handler(void)
{
	
  volatile int clear ;
  if (TIMER0->MIS & 0x1 )//if timeot has occured	//masked interrupt status register -Timer1A timeout flag
	{
			GPIOF->DATA = BLUE;
		TIMER0->ICR = 0x1 ; // clear the Timer1A interrupt timeout flag
		clear = TIMER0->ICR ; //a read to force clearing of interrupts flag
	}
	
}
void adc_init(void)
{
	SYSCTL->RCGCADC |= 0x01 ; //clock for adc 0
	SYSCTL->RCGCGPIO |= 0x08;  //clock for pD
	//pin initialization
	GPIOD->AFSEL |= 0x01 ;  //pD0
	GPIOD->DEN   &= ~0x01;  
	GPIOD->AMSEL = 0x01 ;
	
	//ss configuration
	ADC0->ACTSS &= ~0x08 ; //disable adc
	ADC0->EMUX =  0x4000 ; //GPIO to trigger ADC ss3
  ADC0->SSMUX3 = 7 ;//AIN7 input source 
	ADC0->SSCTL3 = 0x06 ; //end of sequence ,interrupt enable
  ADC0->ACTSS |= 0x08 ; //enable adc ss
}

//function to generate GPIO interrupt when BLUE LED goes on
//this interrupt triggers ADC conversions
void gpio_interrupt (void)
{
	
	GPIOF->IS &= ~BLUE ; //set as edge sensitive
  GPIOF->IBE &= ~BLUE ;//set as a single edge(not both edges)
	GPIOF->IEV |= BLUE ; //set to trigger on Rising edge
	GPIOF->IM = BLUE ; //mask the interrupt
	NVIC->ISER[0] = (1U<<30); //Enable gpiof interrupt
}
void GPIOF_Handler(void)
{
	
		adc_read(); //trigger adc conversion
		GPIOF->ICR |= BLUE ; //clear interrupt
	
	
}
//function to read adc value 
//it is triggerred by pF blue
void adc_read(void)
{
	GPIOF->ADCCTL = BLUE ;
	adc_value = ADC0->SSFIFO3 ;
	while((ADC0->RIS & 0x08) == 0 ) {} //wait for conversion to complete
  LED_control(adc_value);
	
}
// Control LED based on ADC value
void LED_control(uint16_t adcValue) {
    if (adcValue > 2048) {
        GPIOF->DATA |= RED;  // Turn on the LED
    } else {
        GPIOF->DATA &= ~RED; // Turn off the LED
    }
		ADC0->ISC = 0x0008; //clear ss3 interrupts
}

