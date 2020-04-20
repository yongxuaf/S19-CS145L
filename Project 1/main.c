/*
 * GccApplication1.c
 *
 * Created: 4/10/2019 5:01:30 PM
 * Author : Jack Ramirez
 */ 
#include "avr.h"
#include <avr/io.h>
//#include <stdio.h>

void wait(){
	volatile int i, n = 25000;
	for(i = 0; i <n; i++){
		;
	}
	
}

#define XTAL_FRQ_1 1000000lu

void
wait_2(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ_1 / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

int main(void)
{

	CLR_BIT(DDRB, 0);
	SET_BIT(DDRA, 0);
	while(1){
		if (GET_BIT(PINB, 0)){
			CLR_BIT(PORTA, 0);
		}
		else{
				SET_BIT(PORTA, 0);
				//wait_2(500); //without crystal
				wait_avr(500); //with crystal
				CLR_BIT(PORTA,0);
				wait_avr(500);
				//wait_2(500); 

			
		}
	

	}
	
	
	return 0;
}

