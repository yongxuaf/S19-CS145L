/*
 * GccApplication4.c
 *
 * Created: 5/24/2019 1:03:04 PM
 * Author : Jack Ramirez
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <limits.h>

typedef struct stats {
	unsigned short min;
	unsigned short max;
	unsigned short avg;
	unsigned short current; //instant
} stats;

static int sampling_time = 500; //ms

static stats statistics;
static char line1[16];
static char line2[16];

unsigned long sum_of_all_samples;
unsigned long num_of_samples;



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

unsigned short start_sampling(){
	CLR_BIT(ADMUX,0);
	CLR_BIT(ADMUX,1);
	CLR_BIT(ADMUX,2);
	CLR_BIT(ADMUX,3);
	CLR_BIT(ADMUX,4);

	SET_BIT(ADCSRA,6); //starts ADC conversion

	while(!GET_BIT(ADCSRA,4)){}

	SET_BIT(ADCSRA,4);

	return (ADC);
}

void update_stats(unsigned short sample){

	if(num_of_samples >= ULONG_MAX){
		num_of_samples = 0;
	}

	if(sum_of_all_samples >= ULONG_MAX){
		sum_of_all_samples = 0;
	}

	
	num_of_samples++;
	
	sum_of_all_samples += sample;
	
	statistics.current = sample;
	
	if(sample < statistics.min){
		statistics.min = sample;
	}
	
	if(sample > statistics.max){
		statistics.max = sample;
	}
	
	
}
void show_stats(){
	lcd_clr();

	
	unsigned int conversion = (((unsigned long) (statistics.min)) * 5 * 100) / 1023;
	unsigned int digit1 = conversion / 100;
	unsigned int decimals1 = conversion % 100;
	
	unsigned int conversion2 = (((unsigned long) (statistics.max)) * 5 * 100) / 1023;
	unsigned int digit2 = conversion2 / 100;
	unsigned int decimals2 = conversion2 % 100;
	
	snprintf(line1,16,"Mn:%d.%02d Mx:%d.%02d",digit1,decimals1,digit2,decimals2);
	
	conversion = (((unsigned long) (statistics.current)) * 500) / 1023;
	digit1 = conversion / 100;
	decimals1 = conversion % 100;
										//avg
	conversion2 = (((unsigned long) (sum_of_all_samples / num_of_samples)) * 500) / 1023;
	digit2 = conversion2 / 100;
	decimals2 = conversion2 % 100;

	snprintf(line2,16,"V:%d.%02d A:%d.%02d",digit1,decimals1,digit2,decimals2);
	
	
	lcd_pos(0,0);
	lcd_puts2(line1);
	lcd_pos(1,0);
	lcd_puts2(line2);
	
}


int main(void)
{
    avr_init();
	lcd_init();
	lcd_clr();
	
	CLR_BIT(ADMUX,7);
	SET_BIT(ADMUX,6); //AVCC with external capacitor at AREF pin
	
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
	// ADC Control and Status Register A – ADCSRA
	// ADEN: ADC Enable
	// ADPS2:0: ADC Prescaler Select Bits = These bits determine the division factor between the XTAL frequency and the input clock to the ADC
	//			division factor = 64
	
	statistics.min = 1023;
	statistics.max = 0;
	statistics.current = 0;
	
	num_of_samples = 0;
	sum_of_all_samples = 0;
	
	int sample_start = 0;
	int reset = 0;
	
    while (1) 
    {
		int key = get_key();
		if(key){
			switch (key)
			{
				case 1:
					//reset sampling
					sample_start =0;
					reset = 1;
					break;
				case 2:
					//start sampling
					sample_start = 1;
					reset = 0;
					break;
				default: break;
			}
		}
		
		if(sample_start){
			avr_wait(sampling_time);
			unsigned short sample = start_sampling();
			update_stats(sample);
			show_stats();
		}
		
		if(reset){
			num_of_samples = 0;
			sum_of_all_samples = 0;
			statistics.min = 1023;
			statistics.max = 0;
			statistics.current = 0;
			lcd_clr();
		}
	}
}

