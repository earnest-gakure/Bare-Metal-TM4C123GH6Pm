//using processor as event trigger source
//!!!code works

/*****ADC MAPPINGS  LINES************
//AIN0      ADC input 0       PE3
//AIN1      ADC input 1     PE2
//AIN2      ADC input 2       PE1
//AIN3      ADC input 3     PE0
//AIN4      ADC input 4       PD3
//AIN5      ADC input 5     PD2
//AIN6      ADC input 6        PD1
//AIN7      ADC input 7     PD0
//AIN8      ADC input 8       PE5
//AIN9      ADC input 9     PE4
//AIN10      ADC input 10     PB4
//AIN11      ADC input 11   PB5

*/

#include "TM4C123.h"                    /* Device header */

int main()
{
	int adc_result;
  SYSCTL->RCGCADC |= 0x02 ;//clock for adc1
	SYSCTL->RCGCGPIO |= 0x30;//clock for pE ,pF
	
	
	//led configuration
	GPIOF->DIR |= 0x08;
	GPIOF->DEN |= 0x08;
	
	//ADC initialization
	GPIOE->AFSEL |= 0x02; //pE1
	//GPIOE->DIR &= ~0x02; //inputpin
	GPIOE->DEN &= ~0x02; //digital disable
	GPIOE->AMSEL |= 0x02; //disable analog isolation
	
	//ss configuration
	ADC1->ACTSS &= ~0x08; //disables ss
	ADC1->EMUX &= ~0xF000; //processor is the trigger source
	ADC1->SSMUX3 = 2; //input source i.e AIN x of respective pin
	ADC1->SSCTL3 = 0x06;  //end of sequence ,sample interrupt enable
	ADC1->ACTSS |= 0x08 ;//enable ss
	///ADC1->IM = 0x08 ;//mask interrupt
	///ADC1->ISC = 0x08 ;//clear interrupt
	
	  while (1)
		{
			//start conversion
	ADC1->PSSI = 0x08; //on ss3
	adc_result = ADC1->SSFIFO3 ;
			while((ADC1->RIS & 0x08) == 0){}//raw interrupt on the ss //wait for interrupt
			if(adc_result > 2048)
			{
				GPIOF->DATA |= 0x08; //led on
			}
			else{
				GPIOF->DATA &= ~0x08; //led off
			}
			ADC1->ISC = 0x08 ;//clear interrupt
		}
	
}
