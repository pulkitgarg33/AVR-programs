/*
 * rfid_id_checking_via_interrupts.c
 *
 * Created: 01-07-2017 15:41:16
 *  Author: pulki
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>



#define F_CPU 8000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include Delay header file */


//*************************************************************** LCD functions ***************************************************


#define LCD_Dir  DDRD			/* Define LCD data port direction */
#define LCD_Port PORTD			/* Define LCD data port */
#define RS PD2				/* Define Register Select pin */
#define EN PD3 				/* Define Enable signal pin */


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
	LCD_Command(0x0e);              /* Display on cursor off*/
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
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}



//*************************************************************  USART functions**********************************************

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1




void USART_Init( unsigned int ubrr)
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);    //it sets the size of the data and the number of stop bits
}




void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}




unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	
	_delay_us(100);
	/* Get and return received data from buffer */
	return UDR;
	
}


/*   ****************************************************    Interrupt function		*******************************************			*/

int i = 0 ;
char a[9];
int check = 0;

ISR (USART_RXC_vect)
{
	if(i<9)
	{
		a[i] = USART_Receive();
		LCD_Char(a[i]);
		i++;
		
		if(i == 9)
		{check = 1;}
	}
	
}



//********************************************************* Main function  *******************************************************




int main()
{
	
	LCD_Init();
	
	_delay_ms(10);
	
	USART_Init(51);
	
	UCSRB |= (1 << RXCIE);				//rx interrupt enabled
	
	sei();	
	LCD_String("welcome");							//global interrupt enabled
	_delay_ms(1000);
	LCD_Clear();
	
	
	
    char b[9] = "0D007F80B";			//rfid card code
	LCD_Command(0x80);
	LCD_String("place your id");
	
	while(1)
	
	{	LCD_Clear();
		
		LCD_Command(0xc0);
		
			if(check == 0)
			{	
				
				//*************************************   TEMPERATURE DETECTION **********************************************************
				
				
				
				int a = 0;
				ADMUX = 0x00;		//A0 channel is selected
				ADCSRA = 0x87;			//adc is enabled with the prescaler of 128
				ADCSRA |= 0x40;			//conversion starts
				
					if(!(ADCSRA & 0x40))
					
					{
						ADCSRA |= 0x40;				//conversion starts again
						
					}
					
					a = ADC;
					
					float temp = (a*500.0)/1024.0;
					LCD_Clear();
					
					_delay_ms(10);
					a = temp *100;
					
					LCD_Command(0x80);
					LCD_Num(a/100,0x80);
					LCD_Command(0x83);
					LCD_Char('.');
					LCD_Num(a%100,0x84);
					LCD_Command(0x87)	;
					LCD_String("deg C");
				
				LCD_Command(0xc0);
				LCD_String("place your id");
				LCD_Command(0xc0);
				_delay_ms(200);
				
					if(a>7000)
					{
						PORTA &= ~(0x40);
					}
					else
					{
						PORTA |= (1 << PA6);
					}
	
	//	***********************			LIGHT DETECTION CHECK		************************************
				
				int b = 0;
				ADMUX = 0x01;		//A1 channel is selected
				ADCSRA = 0x87;			//adc is enabled with the prescaler of 128
				ADCSRA |= 0x40;			//conversion starts
				
				if(!(ADCSRA & 0x40))
				
				{
					ADCSRA |= 0x40;				//conversion starts again
					
				}
				
				b = ADC;
				
				if(b>700)
				{
					PORTA |= (1 << PA7); 
				}
				else
				{
					PORTA &= ~(0x80);
				}
				
				
				
			}
		
			if ( check == 1)
			{			_delay_ms(500);
						LCD_Clear();
						int flag = 0;
						 i=0;
						check = 0; 
						
						while(i<9)
						{
							
							
							if(a[i] != b[i])
							{
								flag = 1;
							}
							i++;
						}
						
						
						if(flag == 1)					//anuathorized access
						{
							LCD_Clear();
							LCD_Command(0xc0);
							LCD_String("not valid");
							_delay_ms(500);
							LCD_Clear();
							
						}
						
						if(flag == 0)						//authorized access
						{
							LCD_Clear();
							LCD_Command(0xc0);
							LCD_String("welcome");
							_delay_ms(500);
							LCD_Clear();
						}
						
						i=0;
			}

	}

}