/*
 * final_project_1.c
 *
 * Created: 11/13/2019 6:11:58 PM
 * Author : shing
 */ 

#include <avr/io.h>
#include "timer.h"
#include "lcd.h"
#include "pwm.h"
#include "bit.h"
#include "usart.h"
#include <util/delay.h> 
unsigned short xx = 0x00;
unsigned short y = 0x00;
unsigned char command = 0x00;

char buffer[10];

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

short ReadADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX= ADMUX & 0b11111000;
	ADMUX|=ch;
	

	//Start Single conversion

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in io as said in datasheets.
	//The code writes '1' but it result in setting bit to '0' !!!

	//ADCSRA|=(1<<ADIF);

	return ADC;
}

enum x_states { wait, left, right} x_state;
void x_direction(){
	switch(x_state){
		case wait:
			xx = ReadADC(0);
			sprintf(buffer, "X=%d        Wait", xx);
			//LCD_DisplayString(1,buffer);
		//LCD_WriteData(x);
		if(xx > 560 || GetBit(PIND,1) == 1){
			//set_PWM(20);
			//speed(xx);
			x_state = right;
			command = 0x00;
		}
		else if( xx < 535 ||  GetBit(PIND,2) == 1){
			//set_PWM(100);
			//speed(xx);
			x_state = left;
			command = 0x00;
		}
		else{
			x_state = wait;
			command = 0x00;
		}
		break;
		case left:
		xx = ReadADC(0);
		sprintf(buffer, "X=%d      Left", xx);
	//	LCD_DisplayString(1,buffer);
		if(xx > 560||  GetBit(PIND,1) == 1){
			//speed(xx);
			//set_PWM(20);
			x_state = right;
			command = 0x00;
		}
		else if( xx < 535||  GetBit(PIND,2) == 1){
			//speed(xx);
			//set_PWM(100);
			x_state = left;
			command = 0x00;
			//speed();
		}
		else{
			x_state = wait;
			command = 0x00;
		}
		break;
		case right:
		xx = ReadADC(0);
		sprintf(buffer, "X=%d      Right", xx);
		//LCD_DisplayString(1,buffer);
		if(xx > 560||  (GetBit(PIND,1) == 1)){
			//set_PWM(20);
			//speed(xx);
			//x_state = right;
			command = 0x00;
		}
		else if( xx < 535||  (GetBit(PIND,2) == 1)){
			//set_PWM(100);
			//speed(xx);
			x_state = left;
			command = 0x00;
		}
		else{
			x_state = wait;
			command = 0x00;
		}
		break;
		default:
		x_state = wait;
		break;
	}
	switch(x_state){
		case wait:
		PORTC =SetBit(PORTC,0,0);
		PORTC =SetBit(PORTC,2,0);
				PORTC =SetBit(PORTC,1,0);
		PORTC =SetBit(PORTC,3,0);
		break;
		case left:
	//	speed(xx);
		PORTC =SetBit(PORTC,0,1);
		PORTC =SetBit(PORTC,2,1);
		break;
		case right:
	//	speed(xx);
		PORTC =SetBit(PORTC,3,1);
		PORTC =SetBit(PORTC,1,1);
		break;
		default:
		x_state = wait;
		break;
	}
}
void speed(unsigned short x){
	if((x >580 && x < 800) || (x < 525 && x > 150))
	{
		set_PWM(40);
	}
	else if((x >= 800 && x <1100) || (x <= 150 && x > 0))
	{
		set_PWM(40);
	}
	else{
	}
}

char buffer1[10];
int main(void)
{
	DDRD = 0xC0; PORTD = 0x3F;
	//DDRA = 0xFF; PORTA = 0x00;
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0x7F; PORTC = 0x80;
	ADC_init();
	TimerSet(100);
	TimerOn();
	//initUSRT(0);
	set_PWM(30);
	PWM_on();
	
    while (1) 
    {
		//if(USART_HasReceived(0) == 1){
			//command = USART_Receive(0);
			//USART_Flush(0);
		//}
		if((GetBit(PINC,7) == 0) || (GetBit(PIND,0) == 1)){								//Works
			PORTB = SetBit(PORTB,0,1);
			PORTD = 0x80;
			delay_ms(50);
			PORTD= 0x00;
			command = 0x00;
		}
		else if((GetBit(PIND,0) == 1)){
			PORTC =SetBit(PORTC,0,1);
			PORTC =SetBit(PORTC,2,1);
			delay_ms(50);
			PORTC =SetBit(PORTC,0,0);
			PORTC =SetBit(PORTC,2,0);
		}
		else if((GetBit(PIND,0) == 1)){
			PORTC =SetBit(PORTC,3,1);
			PORTC =SetBit(PORTC,1,1);
			delay_ms(50);
			PORTC =SetBit(PORTC,3,0);
			PORTC =SetBit(PORTC,1,0);
		}
		else{
			PORTB = SetBit(PORTB,0,0);
		}
		x_direction();
		//speed(xx);

		while(!TimerFlag);
		TimerFlag = 0;
    }
}

