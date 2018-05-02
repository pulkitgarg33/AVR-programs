/*
 * fading_in_and_fading_out_led_using_pwm.c
 *
 * Created: 15-06-2017 00:58:44
 *  Author: pulki
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>

int main(void)
{
	DDRB = 0xff;
	TCCR0 = 0x6a;
	TCNT0 = 6;
	int flag = 0;
	int count = 0;
    while(1)
    {
		if (TIFR & 0x01 && count == 400)
		{
        if(TIFR & 0x01 && flag == 0)
		{
			TIFR = 0x01;
			TCNT0 = 6;
			OCR0 = OCR0 + 5;
			
			if( OCR0 == 255)
			   flag = 1;
		}
		
		if(TIFR & 0x01 && flag == 1)
		{
			TIFR = 0x01;
			TCNT0 = 6;
			OCR0 = OCR0 - 5;
			
			if( OCR0 == 0)
			flag = 0;
		}
		
		count = 0;
		
		}
		
		
		if (TIFR & 0x01 && count<400)
		{   TIFR = 0x01;
			TCNT0 = 6;
			count++;
			}
    }
}