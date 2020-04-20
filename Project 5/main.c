/*
 * GccApplication6.c
 *
 * Created: 6/10/2019 11:56:04 AM
 * Author : Jack Ramirez
 */ 

#include <avr/io.h>
#include "avr.h"
#include "keypad.h"
#include "lcd.h"
#include "stack_int.h"
#include "stack_char.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int pos = 0;
char line1[16];
char line2[16];


void append(char* s, char c) {
	int len = strlen(s);
	if(len == 16) return;
	s[len] = c;
	s[len+1] = '\0';
}

void append_and_display(char c){
	append(line1, c);
	lcd_pos(0, 0);
	lcd_puts2(line1);
	pos++;
}

int is_op(char c){
	if(c == '+' || c == '-' || c == '*' || c == '/'){
		return 1;
	}
	return 0;
}

int is_unary(char c){
	if(c == '+' || c == '-') return 1;
	return 0;
}

int priority (char op) {
	if (op < 0) // unary operator
		return 3;
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == '/')
		return 2;
	return -1;
}

void process_op( stack_int *st, char op) {
	if (op < 0) {
	  double l = pop(st);
	  switch (-op) {
	   case '+': push(st, l); break; 
	   case '-': push(st, -l); break;
	}	
	  }	else{
	double r = pop(st);
	double l = pop(st);
	switch (op) {
		case '+': push(st, l + r); break;
		case '-': push(st, l - r); break;
		case '*': push(st, l * r); break;
		case '/': push(st, (double)(l / r)); break;
	}
	}
}




double evaluate(char* s) {
	stack_int st;
	st.top = -1;
	st.maxsize = 10;
	stack_char op;
	op.top = -1;
	op.maxsize = 10;
	int unary = 1;
	for (int i = 0; i < strlen(s); i++) {

		if (is_op(s[i])) {
			char cur_op = s[i];
			//char top_op = pop_c(&op);
			if(unary && is_unary(cur_op)) cur_op = -cur_op;
			while (!isEmpty_c(&op) && (
					(cur_op >= 0 && priority(peek_c(&op)) >= priority(cur_op)) ||
					(cur_op < 0 && priority(peek_c(&op)) > priority(cur_op))
					)) {
				process_op(&st, pop_c(&op));
			}
			push_c(&op, cur_op);
			unary = 1;
		} else {
			double number = 0.0;
			while (i < strlen(s) && isalnum(s[i]))
				number = number * 10.0 + s[i++] - '0';
			--i;
			//return number;
			push(&st, number);
			unary = 0;
		}
	}
    while (!isEmpty_c(&op)) {
	    process_op(&st, pop_c(&op));
    }
    return pop(&st);
}

void parse_input(){
	double number = 0;
	//char a[16];
	//int i = 0;
	//for(int i = 0; i < strlen(line1); i++){
		 //while (i < strlen(line1) && isalnum(line1[i]) && line1[i] != '\0')
		  //     append(a, line1[i++]);
	//	 --i;
	//}
	number = evaluate(line1);
	//number = 0;
	//avr_wait(100);
	//sscanf(line1,"%d", &number);
	lcd_pos(1, 0);
	sprintf(line2, dtostrf(number, 6, 4,"%0lf"));
	lcd_puts2(line2);
	
	
}



int main(void)
{
	avr_init();
	lcd_init();
	lcd_clr();
	int pos = 0;
	
    while (1) 
	{
		int key = get_key();
		if(key){
			char c = key + '0';

			switch(key){
				case 1:
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 2:
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 3:
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 4: // 'A'
					append_and_display('+');
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 5: //4
					c = key-1 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 6: //5
					c = key-1 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 7: //6
					c = key-1 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 8: // 'B'
					append_and_display('-');
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;	
				case 9: //7
					c = key-2 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 10: //8
					c = key-2 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 11: //9
					c = key-2 + '0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 12: // 'C'
					append_and_display('*');
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;															
				case 13:  // '*'
					lcd_clr();
					memset(line1, 0, sizeof(line1));
					memset(line2, 0, sizeof(line2));
					break;
				case 14: //0
					c ='0';
					append_and_display(c);
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;
				case 15: //'#'
					parse_input();
					avr_wait(150);
					break;
				case 16: //'D'
					append_and_display('/');
					avr_wait(150);
					if(pos > 16) pos = 0;
					break;				
					
				default: break;
			}
		}
		
		
		
    }
}

