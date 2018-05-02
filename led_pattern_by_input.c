/*
 * led_pattern_by_input.c
 *
 * Created: 13-06-2017 01:05:24
 *  Author: pulki
 */ 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRC = 0x00;
	DDRD = 0xFF;
	PORTC = 0x0F;

	int a=0;

	while(1)
	{

		if( !(PINC & 0x01))
		a=1;

		if( !(PINC & 0x02))
		a=2;

		if( !(PINC & 0x04))
		a=3;
		
		if( !(PINC & 0x08))
		a=4;

		switch(a)
		{
			
			case 1 :
			
			{
				
				
				int i;
				while(1)
				{

					if( !!(PINC & 0x01))
					break;
					PORTD=0x01;
					_delay_ms(500);
					
					for(i=0;i<7;i++)
					{
						PORTD=PORTD<<1;
						_delay_ms(500);
						
					}
				}    }

				break;
				
				

				case 2 :

				{    if( !!(PINC & 0x02))
					break;

					unsigned char a = 0x01;
					int i;
					int flag = 0;
					
					while(1)
					{
						if( !!(PINC & 0x02))
						break;

						if(a == 0x01)
						{  if(flag == 0)
							{PORTD = a;
								_delay_ms(500);
								flag =1;
							}

							for(i=0;i<7;i++)
							{
								a=a<<1;
								PORTD = a;
								_delay_ms(500);

							}
							
						}

						if( a == 0x80 )
						{
							PORTD = a;
							

							for(i=0;i<7;i++)
							{
								a=a>>1;
								PORTD = a;
								_delay_ms(500);

							}
							
						}
					}

				}

				break;



				case 3 :

				{


					int i;
					unsigned char a = 0x01;
					unsigned char b = 0x80;
					int flag = 0;
					
					while(1)
					{

						if( !!(PINC & 0x04))
						break;

						if( a == 0x01 && b == 0x80)
						{  if(flag==0)
							{ PORTD = (a|b);
								_delay_ms(500);
								flag = 1;
							}

							for(i=0;i<3;i++)
							{
								a=a<<1;
								b=b>>1;
								PORTD = (a|b);
								_delay_ms(500);
								
							}
						}


						if ( a == 0x08 && b == 0x10)
						{
							for(i=0;i<3;i++)
							{  a=a>>1;
								b=b<<1;

								PORTD = (a|b);
								_delay_ms(500);
								
								
							}
							
							
						}
						
					}
				}

				break;



				default : break;
				
				
				
				
				
				
				
			}

		}


		
		

	}
