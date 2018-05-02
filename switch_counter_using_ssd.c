/*
 * switch_counter_using_ssd.c
 *
 * Created: 13-06-2017 00:29:19
 *  Author: pulki
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
		DDRC = 0x00;
		DDRD = 0xff;
		DDRB = 0xff;
		unsigned char a[] = { 0x3F , 0x06 , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D , 0x07 , 0x7F , 0x67 } ;
			int i = 0;
			PORTC = 0xff;
    while(1)
    {
        if ( !(PINC & 0x01))
		{
			while(!(PINC & 0x01))
			{
				PORTB = ~(0x01);
				PORTD = a[i/100];
				
				_delay_ms(10);
				
				PORTB = ~(0x02);
				PORTD = a[(i%100)/10];
				
				_delay_ms(10);
				
				PORTB = ~(0x04);
				PORTD = a[i%10];
				
				_delay_ms(10);
			}
			
			i++;
			
		}
		
		
		        if ( !(PINC & 0x02))
		        {
			        while(!(PINC & 0x02))
			        {
				       PORTB = ~(0x01);
				       PORTD = a[i/100];
				       
				       _delay_ms(10);
				       
				       PORTB = ~(0x02);
				       PORTD = a[(i%100)/10];
				       
				       _delay_ms(10);
				       
				       PORTB = ~(0x04);
				       PORTD = a[i%10];
				       
				       _delay_ms(10); 
			        }
			        
			        i--;
			        
		        }
				
				
				PORTB = ~(0x01);
				PORTD = a[i/100];
				
				_delay_ms(5);
				
				PORTB = ~(0x02);
				PORTD = a[(i%100)/10];
				
				_delay_ms(5);
				
				PORTB = ~(0x04);
				PORTD = a[i%10];
				
				_delay_ms(5);
    }
}