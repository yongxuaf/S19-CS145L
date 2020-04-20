/*
 * GccApplication3.c
 *
 * Created: 4/22/2019 3:15:06 PM
 * Author : Jack Ramirez
 */ 

#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>

#define DATE_SIZE 13
#define TIME_SIZE 14

struct TimeDate {
	volatile int year;
	volatile int month;
	volatile int day;
	
	volatile int hour;
	volatile int minute;
	volatile int second;
	volatile int milli;
	
	volatile int military;
	
	char time_str[TIME_SIZE];
	char date_str[DATE_SIZE];
};


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

void output_time(struct TimeDate *tm){
	snprintf(tm->date_str, DATE_SIZE, "%02d/%02d/%04d", tm->month, tm->day, tm->year);
	if(!tm->military){
		int temp = tm->hour < 12 ? tm->hour : (tm->hour % 12);
		temp = temp == 0 ? 12 : temp;
		snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d %s", temp, tm->minute, tm->second, tm->hour < 12 ? "AM" : "PM");
	}else{
		snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d   ", tm->hour, tm->minute, tm->second);
	}
	lcd_pos(0, 0);
	lcd_puts2(tm->date_str);
	lcd_pos(1, 0);
	lcd_puts2(tm->time_str);
}



void blink(struct TimeDate *tm, int row, int col, int inc, int dec){
	char dblank[DATE_SIZE];
	char tblank[TIME_SIZE];
	if(!row) { //row == 0 -> date
		switch(col){
			case 0:
					if(inc) tm->month = tm->month + 1;
					else if(dec) tm->month = tm->month - 1;
					lcd_pos(row, col);
					snprintf(dblank, DATE_SIZE, "  /%02d/%04d", tm->day, tm->year);
					lcd_puts2(dblank);
 					avr_wait(500);
					lcd_pos(row, col);
					snprintf(tm->date_str, DATE_SIZE, "%02d/%02d/%04d", tm->month, tm->day, tm->year);
 					lcd_puts2(tm->date_str);
 					break;
			case 1:
					if(inc) tm->day = tm->day + 1;
					else if(dec) tm->day = tm->day - 1;
					lcd_pos(row, col-1);
					snprintf(dblank, DATE_SIZE, "%02d/  /%04d", tm->month, tm->year);
					lcd_puts2(dblank);
					avr_wait(500);
					lcd_pos(row, col-1);
					snprintf(tm->date_str, DATE_SIZE, "%02d/%02d/%04d", tm->month, tm->day, tm->year);
					lcd_puts2(tm->date_str);
					break;
			case 2:
					if(inc) tm->year = tm->year + 1;
					else if(dec) tm->year = tm->year - 1;
					lcd_pos(row, col-2);
					snprintf(dblank, DATE_SIZE, "%02d/%02d/    ", tm->month, tm->day);
					lcd_puts2(dblank);
					avr_wait(500);
					lcd_pos(row, col-2);
					snprintf(tm->date_str, DATE_SIZE, "%02d/%02d/%04d", tm->month, tm->day, tm->year);
					lcd_puts2(tm->date_str);
					break;

		}
	}else{ //row == 1 -> time
		if(!tm->military){
			int temp = tm->hour < 12 ? tm->hour : (tm->hour % 12);
			temp = temp == 0 ? 12 : temp;
			switch(col){
				case 0:
					if(inc) tm->hour = tm->hour + 1;
					else if(dec) tm->hour = tm->hour - 1;
// 						int temp = tm->hour < 12 ? tm->hour : (tm->hour % 12);
// 						temp = temp == 0 ? 12 : temp;
					lcd_pos(row, col);
					snprintf(tblank, TIME_SIZE,"  :%02d:%02d %s", tm->minute, tm->second, tm->hour < 12 ? "AM" : "PM");
					lcd_puts2(tblank);
					avr_wait(500);
					lcd_pos(row, col);
					snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d %s", temp, tm->minute, tm->second, tm->hour < 12 ? "AM" : "PM");
					//tm->hour = temp;

					lcd_puts2(tm->time_str);
					break;
				case 1:
					if(inc) tm->minute = tm->minute + 1;
					else if(dec) tm->minute = tm->minute - 1;
					lcd_pos(row, col-1);
					snprintf(tblank, TIME_SIZE,"%02d:  :%02d %s", temp, tm->second, tm->hour < 12 ? "AM" : "PM");
					lcd_puts2(tblank);
					avr_wait(500);
					lcd_pos(row, col-1);
							snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d %s", temp, tm->minute, tm->second, tm->hour < 12 ? "AM" : "PM");

					lcd_puts2(tm->time_str);
					break;
				case 2:
					if(inc) tm->second = tm->second + 1;
					else if(dec) tm->second = tm->second - 1;	
					lcd_pos(row, col-2);
					snprintf(tblank, TIME_SIZE,"%02d:%02d:   %s", temp, tm->minute, tm->hour < 12 ? "AM" : "PM");
					lcd_puts2(tblank);
					avr_wait(500);
					lcd_pos(row, col-2);
							snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d %s", temp, tm->minute, tm->second, tm->hour < 12 ? "AM" : "PM");

					lcd_puts2(tm->time_str);
					break;
			}
		}else{
			switch(col){
				case 0:
				if(inc){
					 tm->hour = tm->hour + 1;
					 if(tm->hour > 23) tm->hour = 0;
				}
				else if(dec) {
					tm->hour = tm->hour - 1;
					if(tm->hour < 0) tm->hour = 23;
				}
				lcd_pos(row, col);
				snprintf(tblank, TIME_SIZE,"  :%02d:%02d", tm->minute, tm->second);
				lcd_puts2(tblank);
				avr_wait(500);
				lcd_pos(row, col);
						snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d", tm->hour, tm->minute, tm->second);

				lcd_puts2(tm->time_str);
				break;
				case 1:
				if(inc) {
				tm->minute = tm->minute + 1;
				if(tm->minute > 59) tm->minute = 0;
				}
				else if(dec) {tm->minute = tm->minute - 1;
				if(tm->minute < 0) tm->minute = 59;
				}
				lcd_pos(row, col-1);
				snprintf(tblank, TIME_SIZE,"%02d:  :%02d", tm->hour, tm->second);
				lcd_puts2(tblank);
				avr_wait(500);
				lcd_pos(row, col-1);
						snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d", tm->hour, tm->minute, tm->second);

				lcd_puts2(tm->time_str);
				break;
				case 2:
				if(inc) {
					tm->second = tm->second + 1;
					if(tm->second > 59) tm->second = 0; 
				}
				else if(dec) {
					tm->second = tm->second - 1;
					if(tm->second < 0) tm->second = 59;
				}
				lcd_pos(row, col-2);
				snprintf(tblank, TIME_SIZE,"%02d:%02d:  ", tm->hour, tm->minute);
				lcd_puts2(tblank);
				avr_wait(500);
				lcd_pos(row, col-2);
						snprintf(tm->time_str, TIME_SIZE, "%02d:%02d:%02d", tm->hour, tm->minute, tm->second);

				lcd_puts2(tm->time_str);
				break;
			}
	}

	}
}

int leap_year(struct TimeDate *tm){
	return tm->year % 4 == 0 && (tm->year % 100 != 0 || tm->year % 400 == 0);
}

void inc_month(struct TimeDate *tm){
	if(tm->day > 28 && tm->month == 2 && !(leap_year(tm))){
		tm->day = 1;
		tm->month++;
		}else if(tm->day > 29 && tm->month == 2){
		tm->day = 1;
		tm->month++;
		}else if(tm->day > 30 && (tm->month == 4 || tm->month == 6 || tm->month == 9 || tm->month == 11)){
		tm->day = 1;
		tm->month++;
		}else if(tm->day > 31){
		tm->day = 1;
		tm->month++;
	}
}

void start_clock(struct TimeDate *tm){
	if(tm->second < 60){
		tm->second++;
		if(tm->second >= 60){
			tm->second = 0;
			tm->minute++;
		}
	}
	if(tm->minute >= 60){
		tm->minute = 0;
		tm->hour++;
	}
	if(tm->hour >= 24){
		tm->hour = 0;
		tm->day++;
	}
	if(tm->day >= 28){
		inc_month(tm);
	}
	if(tm->month >= 13){
		tm->month = 1;
		tm->year++;
	}
}





/*
	KEYPAD INPUTS:
	
	Button (number) - operation
	
	A (4) - Program mode
		4 (5) - Move field right
		6 (7) - Move field left
		2 (2) - Move field down
		8 (10) - Move field up
		B (8) - Decrement field
		C (12) - Increment field
	2 (2) - Military Time
	1 (1) - Start Clock
	3 (3) - Stop Clock
	
*/



int main(void)
{
    /* Replace with your application code */
	avr_init();
	lcd_init();			//year, mon, day, hr, min, sec, milli, mili, date, time
	struct TimeDate tm = {2020, 12,   31,  23,  59,  58,  100,    1,   " ",  " "};
	//test_lcd();
	int start_time = 0;
    int k = 0, row = 0, col = 0, inc = 0, dec = 0;
	output_time(&tm);
	//avr_wait(500);
	//lcd_clr();
	
	while (1) 
    {
		avr_wait(100);
		k = get_key();
		if(k) {//k is not 0 
			if(k == 4){ //programming mode
				start_time = 0;
				avr_wait(150);
				k = get_key();
				while(k != 4){
						switch(k){
							case 5: col++; break;
							case 7: col--; break;
							case 2: row++; break;
							case 10: row--; break;
							case 8: dec = 1; break;
							case 12: inc = 1; break;
				
						}
						//avr_wait(150);
						blink(&tm, row, col, inc, dec);
						inc = 0;
						dec = 0;
						avr_wait(150);
						k = get_key();
				}

			}
			else if(k == 1){
				//start time
				start_time = !(start_time);
				avr_wait(50);
			}
			 else if(k == 2){
				//military
				tm.military = !(tm.military);
				avr_wait(100);
				//output_time(&tm);
			}
			else if(k== 3){
				start_time = 0;
			}
			
		
		}
		if(start_time){
			//if (k==1)
			//	start_time = 0;
			start_clock(&tm);
			avr_wait(1000);
			output_time(&tm);
		}
		else{
			//start_time = 0;
			avr_wait(50);
			output_time(&tm);
		}
	}
}

