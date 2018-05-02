/*
 * motor_interfacing.c
 *
 * Created: 13-06-2017 01:10:13
 *  Author: pulki
 */ 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


int main()
{
	DDRD = 0xFF;
	DDRC = 0x00;
	PORTC = 0xFF;
	int a=0;

	while(1)
	{
		if(!(PINC & 0x01))
		a=1;

		if(!(PINC & 0x02))
		a=2;

		if(!(PINC & 0x04))
		a=3;

		if(!(PINC & 0x08))
		a=4;



		switch(a)
		{
			case 1 :
			
			
			while(1)
			{
				if(!!(PINC & 0x01))
				break;
				
				PORTD = 0x05;
				
				
			}
			break;



			case 2 :


			while(1)
			{

				if(!!(PINC & 0x02))
				break;

				PORTD = 0x0A;
				
				
			}

			break;



			case 3 :

			while(1)
			{
				if(!!(PINC & 0x04))
				break;

				PORTD = 0x06;
				
			}
			break;



			case 4 :

			while(1)
			{
				if(!!(PINC & 0x08))
				break;


				PORTD = 0x09;
				
			}


			break;



			default : break;





		}

		a=0;
		PORTD = 0x00;


	}



}
