/*
 * stack_char.h
 *
 * Created: 6/10/2019 3:33:53 PM
 *  Author: Jack Ramirez
 */ 


#ifndef STACK_CHAR_H_
#define STACK_CHAR_H_

typedef struct stack_char{
	int maxsize;
	int top;
	char items[16];
}stack_char;

//struct stack_char* newStack_c(int cap);
int size_c(stack_char *ns);
int isEmpty_c(stack_char *ns);
int isFull_c(stack_char *ns);
void push_c(stack_char *ns, char x);
char peek_c(stack_char *ns);
char pop_c(stack_char *ns);



#endif /* STACK_CHAR_H_ */