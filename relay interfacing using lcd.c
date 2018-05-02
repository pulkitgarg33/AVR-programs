/*
 * relay_interfacing_using_lcd.c
 *
 * Created: 19-06-2017 09:40:21
 *  Author: pulki
 */ 


#define F_CPU 8000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */

#define LCD_Dir  DDRD			/* Define LCD data port direction */
#define LCD_Port PORTD			/* Define LCD data port */
#define RS PD0				/* Define Register Select pin */
#define EN PD1 				/* Define Enable signal pin */


void LCD_Command( unsigned char cmnd )
{
	LCD_Port &= ~(1<<RS);		/* RS=0, command reg. */
	LCD_Port &= 0x0f;
	LCD_Port |= (cmnd & 0xf0);// (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	
	LCD_Port |= (1<<EN);		/* Enable pulse */
	_delay_us(1);
	LCD_Port &= ~(1<<EN);

	_delay_us(200);
	
	LCD_Port &= 0x0f;
	LCD_Port |= ((cmnd<<4) & 0xf0);//(LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port |= (1<<RS);		/* RS=1, data reg. */
	LCD_Port &= 0x0f;
	LCD_Port |= (data & 0xf0);//(LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~(1<<EN);

	_delay_us(200);
	
	LCD_Port &= 0x0f;
	LCD_Port |= ((data << 4) & 0xf0);//(LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0xFF;			/* Make LCD port direction as o/p */
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x33);
	LCD_Command(0x32);
	LCD_Command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);              /* Display on cursor off*/
	LCD_Command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              /* Clear display screen*/
	_delay_ms(2);
}


void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	LCD_String(str);		/* Call LCD string function */
}


void LCD_Num(int a, unsigned char pos)
{
	int temp = a;
	int count = 0;
	
	while(temp!=0)
	{
		temp = temp/10;
		count++;
	}
	
	pos = pos + (count-1);
	
	
	while(a!=0)
	{
		LCD_Command(pos);
		LCD_Char(48 + (a%10) );
		a= a/10;
		pos--;
	}
	
}


void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_us(2);
	LCD_Command (0x80);		/* Cursor at home position */
}




int main(void)
{
	DDRD = 0xff;
	DDRB = 0x00;
	DDRA = 0xff;
	
	PORTB = 0xff;
	int a = 4;
	
	
	LCD_Init();
	PORTA = 0x00;
	
	LCD_Clear();
	 
	 
    while(1)
    {
		LCD_Clear();
        	
        	if(!(PINB & 0x01))
        	a =1;
        	
        	if(!(PINB & 0x02))
        	a =2;
        	
        	if(!(PINB & 0x04))
        	a =3;
        	
        	if(!(PINB & 0x08))
        	a =4;
			
			
			switch(a)
			{
				case 1 : 
				
				{
					while(a==1)
					{
						
					PORTA = 0x01;
					LCD_Command(0x80);
					LCD_String("load 1 :");
					LCD_String(" on");
					LCD_Command(0xc0);
					LCD_String("load 2 :");
					LCD_String(" off");
					
						if(!(PINB & 0x02))
						a =2;
						
						if(!(PINB & 0x04))
						a =3;
						
						if(!(PINB & 0x08))
						a =4;
					
					}
				}
				
				break;
				
				case 2 :
				
				{
					while(a==2)
					{
						
					
					PORTA = 0x02;
					
					LCD_Command(0x80);
					LCD_String("load 1 :");
					LCD_String(" off");
					LCD_Command(0xc0);
					LCD_String("load 2 :");
					LCD_String(" on");
					
					if(!(PINB & 0x01))
					a =1;
					
					if(!(PINB & 0x04))
					a =3;
					
					if(!(PINB & 0x08))
					a =4;
					
					}
				}
				
				
				break;
				
				
				
								case 3 :
								
								{
									while(a==3)
									
									{
										PORTA = 0x03;
									
									LCD_Command(0x80);
									LCD_String("load 1 :");
									LCD_String(" on");
									LCD_Command(0xc0);
									LCD_String("load 2 :");
									LCD_String(" on");
									
									
									if(!(PINB & 0x01))
									a =1;
									
									if(!(PINB & 0x02))
									a =2;
									
									if(!(PINB & 0x08))
									a =4;
									
									}
								}
								
								break;
								
								
								
												case 4 :
												
												{
													while(a==4)
													{
														
													
													PORTA = 0x00;
													
													LCD_Command(0x80);
													LCD_String("load 1 :");
													LCD_String(" off");
													LCD_Command(0xc0);
													LCD_String("load 2 :");
													LCD_String(" off");
													
													if(!(PINB & 0x01))
													a =1;
													
													if(!(PINB & 0x02))
													a =2;
													
													if(!(PINB & 0x04))
													a =3;
													
													}
												}
												
												break;
												
												
												
												default :  break;
			}
			
			
			
			
    }
}