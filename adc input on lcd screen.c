/*
 * adc_input_on_lcd_screen.c
 *
 * Created: 16-06-2017 10:55:38
 *  Author: pulkit
 */ 



#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define rs PC0
#define e PC1

/* Function Prototype */

void init(); //for initialising LCD
void cmd(unsigned char); // for passing cmd
void dat(unsigned char); //for passing data

/*******************/

/* Function definition */

void init()
{
	cmd(0x38);  //lcd in 8-bit mode (0x28 for 4-bit mode)
	cmd(0x0e);  //lcd to enable cursor
	cmd(0x06);  //shifting the cursor from left to right
	cmd(0x80);  //first row first col
	cmd(0x01);  //clear screen
}

void cmd(unsigned char cd)
{
	PORTD=cd;   //8-bit data
	PORTC=(1<<e);  //enable is high, rs=0 for cmd
	_delay_us(100);
	PORTC&=~(1<<e);  //enable is low
}

void dat(unsigned char da)
{
	PORTD=da;   //8-bit data
	PORTC=(1<<e)|(1<<rs);   //enable is high, rs=1 for data
	_delay_us(100);
	PORTC&=~(1<<e);   //enable is low
}


void num(int a, unsigned char pos)
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
		cmd(pos);
		dat(48 + (a%10) );
		a= a/10;
		pos--;
	}
	
}

void str(unsigned char *a)
{
	while(*a!='\0')
	{
		dat(*a);
		a++; //incrementing the address
	}
}





int main(void)
{
	DDRA = 0x00;
	DDRC = 0xff;
	DDRD = 0xff;
	int a = 0;
	
	ADMUX = 0x01;		//A0 channel is selected
	ADCSRA = 0x87;			//adc is enabled with the prescaler of 128
	ADCSRA |= 0x40;			//conversion starts
	
	init();
	_delay_us(100);
	
	
	while(1)
	{
		if(!(ADCSRA & 0x40))
		
		{
			ADCSRA |= 0x40;				//conversion starts again
			
		}
		
		a = ADC;
		cmd(0x01);
		
		_delay_ms(10);
		
		cmd(0x80);
		num(a,0x80);
		_delay_ms(200);
		
		
		
	}
}