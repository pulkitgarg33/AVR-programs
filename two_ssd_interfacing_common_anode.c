/*
 * two_ssd_interfacing_common_anode.c
 *
 * Created: 13-06-2017 11:08:31
 *  Author: pulki
 */ 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = 0xff;
	DDRD = 0xff;
	unsigned char a[] = { 0x3F , 0x06 , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D , 0x07 , 0x7F , 0x67 } ;
	
	while(1)
	{
		for(int i = 0; i<100 ; i++)
		{
			PORTB = (0x01);
			PORTD = ~(a[i/10]);
			
			_delay_ms(50);
			
			PORTB = (0x02);
			PORTD = ~(a[i%10]);
			
			_delay_ms(50);
			
			
		}
		
		
		
	}
}