/*
 * pwm.h
 *
 * Created: 11/20/2019 6:29:08 PM
 *  Author: shing
 */ 


#ifndef PWM_H_
#define PWM_H_

void set_PWM(double frequency){
	static double current_frequency;
	if(!frequency){
		TCCR3B &= 0x08;
	}
	else{
		TCCR3B |= 0x03;
	}
	if(frequency < 0.954){
		OCR3A = 0xFFFF;
		OCR3B = 0xFFFF;
				}
	else if(frequency > 75000){
		OCR3A = 0x0000;
		OCR3B = 0x0000;
		}
	else{
	OCR3A = (short)(80000000 / (1280000*frequency)) - 1;
	OCR3B = (short)(80000000 / (1280000*frequency)) - 1;
	}
	TCNT3 = 0;

	current_frequency = frequency;
}

void PWM_on(){
	TCCR3A = (1 << COM3A0) | (1 << COM3B0);
	TCCR3B = ( 1<< WGM32)  | (1 <<CS30);
	//set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}



#endif /* PWM_H_ */