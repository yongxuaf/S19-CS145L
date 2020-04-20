/*
 * keypad.c
 *
 * Created: 6/10/2019 12:55:58 AM
 *  Author: Jack Ramirez
 */ 

#include "keypad.h"
#include "avr.h"
#include <avr/io.h>

int is_pressed(int r, int c){
	DDRC = 0;
	PINC = 0;
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	CLR_BIT(DDRC, c+4);
	SET_BIT(PORTC, c+4);
	avr_wait(1);
	if(GET_BIT(PINC, c+4)){return 0;}
	else{ return 1;}
}

int get_key(){
	int r, c;
	for(r = 0; r < 4; r++){
		for(c = 0; c < 4; c++){
			if(is_pressed(r,c)){
				return (r*4 + c) + 1;
			}
		}
	}
	
	return 0;
}
