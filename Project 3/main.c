/*
 * GccApplication2.c
 *
 * Created: 5/6/2019 2:58:08 PM
 * Author : Jack Ramirez
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include "avr.h"
#include "lcd.h"

//#define NUM_SONG 2
//#define NOTES_PER_SONG 10

#define NUM_NOTES 14

// #define W 8
// #define H 4
// #define Q 2
// #define E 1

#define LINE_SIZE 16

#define A_INDEX 0
#define A_SHARP_INDEX 1
#define B_INDEX 2
#define B_SHARP_INDEX 3
#define C_INDEX 4
#define C_SHARP_INDEX 5
#define D_INDEX 6
#define D_SHARP_INDEX 7
#define E_INDEX 8
#define E_SHARP_INDEX 9
#define F_INDEX 10
#define F_SHARP_INDEX 11
#define G_INDEX 12
#define G_SHARP_INDEX 13


#define A_NOTE 440
#define A_SHARP_NOTE 466
#define B_NOTE 493
#define B_SHARP_NOTE 523
#define C_NOTE 554
#define C_SHARP_NOTE 587
#define D_NOTE 622
#define D_SHARP_NOTE 659
#define E_NOTE 694
#define E_SHARP_NOTE 739
#define F_NOTE 783
#define F_SHARP_NOTE 830
#define G_NOTE 880
#define G_SHARP_NOTE 932
//add more notes?

char* translateFreq(int freq){
	switch(freq){
		case 440:
			return "A NOTE ";
		case 466:
			return "A SHARP";
		case 493:
			return "B NOTE ";
		case 523:
			return "B SHARP";
		case 554:
			return "C NOTE ";
		case 587:
			return "C SHARP";
		case 622:
			return "D NOTE ";
		case 659:
			return "D SHARP";
		case 694:
			return "E NOTE ";
		case 739:
			return "E SHARP";
		case 783:
			return "F NOTE ";
		case 830:
			return "F SHARP";
		case 880:
			return "G NOTE ";
		case 932:
			return "G SHARP";
		default: return "?";
			}
}

typedef struct note{
	unsigned int freq;
	unsigned int dura;
	} note;


// note references[NUM_NOTES];
// 
// note song1[8];
// note song2[4];
// 
// note* songs[NUM_SONG];
// 
// static const int songSizes[NUM_SONG] = {8, 4};
	
static char line1[LINE_SIZE];
//static char line2[LINE_SIZE];

static int i = 0;

//pick 2 of 3:
static int pitch_change = 0; //normal
static int tempo_change = 1; //normal
//static int volume_change = 0;




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

// void init_songs(){
// 	song1[0] = references[G_INDEX];
// 	song1[1] = references[A_INDEX];
// 	song1[2] = references[B_INDEX];
// 	song1[3] = references[C_INDEX];
// 	song1[4] = references[D_INDEX];
// 	song1[5] = references[E_INDEX];
// 	song1[6] = references[F_SHARP_INDEX];
// 	song1[7] = references[G_INDEX];
// 
// 	song2[0] = references[G_INDEX];
// 	song2[1] = references[C_INDEX];
// 	song2[2] = references[A_INDEX];
// 	song2[3] = references[D_INDEX];
// 
// 	songs[0] = song1;
// 	songs[1] = song2;
// }



void wait_avr3(unsigned int usec){
	TCCR0 = 2;

	while(usec--){
		TCNT0 = (unsigned char) (256 - (XTAL_FRQ / 8) * 0.000005);
		SET_BIT(TIFR,TOV0);
		WDR();
		while(!GET_BIT(TIFR,TOV0));
	}

	TCCR0 = 0;
}

unsigned int calculateWait(int frequency){
	float period = (1.0 / frequency) / 2.0;
	unsigned int wait = (unsigned int) (round(period * 100000));
	return wait;
}

int parseFrequency(unsigned int wait){
	float recip_freq = (float) (wait * 0.00001 * 2);
	int freq = (int) (1 / recip_freq);
	return freq;
}




void play_note(unsigned int frequency,unsigned int duration){
	int z;
	int k = get_key();
	switch(k){
		case 7:
			tempo_change--;
			if(tempo_change <= 0){
				tempo_change = 1;
			}
			break;
		case 8:
			pitch_change -= 30;
			break;
		case 11:
			tempo_change++;
			break;
		case 12:
			pitch_change += 30;
			break;
		default: break;
	}
	//change pitch:
	char* c= translateFreq(frequency);
	//snprintf(c, LINE_SIZE, "%08c", translateFreq(frequency));
	frequency = frequency + pitch_change;
	
	unsigned int period = (unsigned int) ((100000) / frequency);

	unsigned int tHigh = period / 2;
	unsigned int tLow = period / 2;
	
	//change tempo:
	duration = duration / tempo_change;
	
	lcd_clr();
	snprintf(line1,LINE_SIZE,"Song %02d",i);
	//snprintf(line2, LINE_SIZE, "%03d", frequency);
	//snprintf(line2,LINE_SIZE,"Has %02d Notes",songSizes[index % NUM_SONG]);
	lcd_pos(0,0);
	lcd_puts2(line1);
	lcd_pos(1,0);
	lcd_puts2(c);

	unsigned int cycles = frequency * duration;
	for(z = 0; z < cycles; z++){
		SET_BIT(PORTB,3);
		wait_avr3(tHigh);
		CLR_BIT(PORTB,3);
		wait_avr3(tLow);
	
	}
}


void play_song(note* song,int size){
	int i;

	for(i = 0; i < 8; i++){
		play_note(song[i].freq,song[i].dura);
	}
}


void play_birthday(){

// 
// 	play_note(G_NOTE,2);
// 	play_note(F_NOTE,2);
// 	play_note(E_NOTE,2);
// 	play_note(C_NOTE,1);
// 	play_note(C_NOTE,1);
// 	play_note(B_NOTE,2);
// 	play_note(G_NOTE,2);
// 	play_note(A_NOTE,2);
// 	play_note(G_NOTE,4);
	
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 1);
	play_note(E_NOTE, 2);
	play_note(D_NOTE, 2);
	play_note(G_NOTE, 2);
	play_note(F_SHARP_NOTE, 3);
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 1);
	
	play_note(E_NOTE, 2);
	play_note(D_NOTE, 2);
	play_note(A_NOTE, 2);
	play_note(G_NOTE, 3);
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 2);
	play_note(B_NOTE, 2);
	
	play_note(G_NOTE, 2);
	play_note(F_SHARP_NOTE, 2);
	play_note(E_NOTE, 2);
	play_note(C_NOTE, 1);
	play_note(C_NOTE, 1);
	play_note(B_NOTE, 2);
	play_note(G_NOTE, 2);
	play_note(A_NOTE, 2);
	play_note(G_NOTE, 3);

//25 notes - from Internet


}

void play_song2(){
	play_note(C_NOTE,2);
	play_note(E_NOTE,1);
	play_note(F_SHARP_NOTE,4);
	play_note(D_NOTE,1);


	play_note(G_NOTE,2);
	play_note(C_NOTE,1);
	play_note(A_NOTE,1);
	play_note(D_NOTE,2);
	

}

void play_scale(){
	//C, D, E, F, G, A, B, C
	play_note(C_NOTE, 3);
	play_note(D_NOTE, 3);
	play_note(E_NOTE, 3);
	play_note(F_NOTE, 3);
	play_note(G_NOTE, 3);
	play_note(A_NOTE, 3);
	play_note(B_NOTE, 3);
	play_note(C_NOTE, 3);
}

void play_allnotes(){
	play_note(A_NOTE, 2);
	play_note(A_SHARP_NOTE, 2);
	play_note(B_NOTE, 2);
	play_note(B_SHARP_NOTE, 2);
	play_note(C_NOTE, 2);
	play_note(C_SHARP_NOTE, 2);
	play_note(D_NOTE, 2);
	play_note(D_SHARP_NOTE, 2);
	play_note(E_NOTE, 2);
	play_note(E_SHARP_NOTE, 2);
	play_note(F_NOTE, 2);
	play_note(F_SHARP_NOTE, 2);
	play_note(G_NOTE, 2);
	play_note(G_SHARP_NOTE, 2);

}

void play_twinkleStar(){
	play_note(C_NOTE, 1);
	play_note(C_NOTE, 1);
	play_note(G_NOTE, 1);
	play_note(G_NOTE, 1);

	play_note(A_NOTE, 1);
	play_note(A_NOTE, 1);
	play_note(G_NOTE, 2);

	play_note(F_NOTE, 1);
	play_note(F_NOTE, 1);
	play_note(E_NOTE, 1);
	play_note(E_NOTE, 1);
	
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 1);
	play_note(C_NOTE, 2);
	
	play_note(G_NOTE, 1);
	play_note(G_NOTE, 1);
	play_note(F_NOTE, 1);
	play_note(F_NOTE, 1);	
	
	play_note(E_NOTE, 1);
	play_note(E_NOTE, 1);
	play_note(D_NOTE, 2);

	play_note(G_NOTE, 1);
	play_note(G_NOTE, 1);
	play_note(F_NOTE, 1);
	play_note(F_NOTE, 1);
	
	play_note(E_NOTE, 1);
	play_note(E_NOTE, 1);
	play_note(D_NOTE, 2);
	
	play_note(C_NOTE, 1);
	play_note(C_NOTE, 1);
	play_note(G_NOTE, 1);
	play_note(G_NOTE, 1);

	play_note(A_NOTE, 1);
	play_note(A_NOTE, 1);
	play_note(G_NOTE, 2);

	play_note(F_NOTE, 1);
	play_note(F_NOTE, 1);
	play_note(E_NOTE, 1);
	play_note(E_NOTE, 1);
	
	play_note(D_NOTE, 1);
	play_note(D_NOTE, 1);
	play_note(C_NOTE, 2);
}


// 
// 
// void display_song(note* song, int index){
// 	lcd_clr();
// 	snprintf(line1,LINE_SIZE,"Song %02d",(index % NUM_SONG) + 1);
// 	snprintf(line2,LINE_SIZE,"Has %02d Notes",songSizes[index % NUM_SONG]);
// 	lcd_pos(0,0);
// 	lcd_puts2(line1);
// 	lcd_pos(1,0);
// 	lcd_puts2(line2);
// }


/*
	Song Controls:
	7 - Lower tempo
	11 - Raise tempo
	12 - Raise pitch
	8 - Lower pitch
	
	1 - Birthday song
	2 - Song 2
	3 - All notes
	4 - C Scale
	
*/

int main(void)
{
	
	avr_init();
	lcd_init();
	
	SET_BIT(DDRB,3);
	
	//init_songs();
	
	lcd_clr();
	lcd_pos(0 ,0);
	const char* p = "Pick a song";
	lcd_puts2(p);
	
	//int i = 0;
	
    while (1) 
    {
		int key = get_key();
		if(key){
			//int i = 0;
			
			switch(key){
				//case 1: songIndex = 0;
				//		break;
				case 1:
						i = 1;
						break;
				case 2:
						i = 2;
						break;
				case 3:
						i = 3;
						break;
				case 4:
						i = 4;
						break;
				case 5:
						i = 5;
						break;
// 				case 7:
// 						tempo_change--;
// 						if(tempo_change <= 0){
// 							tempo_change = 1;
// 						}
// 						break;
// 				case 8:
// 						pitch_change -= 20;
// 						break;
// 				case 11:
// 						tempo_change++;
// 						break;
// 				case 12:
// 						pitch_change += 20;
// 						break;
				default: break;
				//add case: change volume, tempo, pitch
				
			}
			//display_song(songs[songIndex],songIndex);
			if(i == 1)
				play_birthday();
			else if(i == 2)
				play_song2();
			else if(i == 3){
					play_allnotes();
				}
			else if(i == 4){
				play_scale();
			}
			else if(i ==5){
				play_twinkleStar();
			}
			else{
				//play_scale();
			}
			
		}
		
    }
}

