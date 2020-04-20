/*
 * stack_int.h
 *
 * Created: 6/10/2019 3:29:34 PM
 *  Author: Jack Ramirez
 */ 


#ifndef STACK_INT_H_
#define STACK_INT_H_

typedef struct stack_int{
	int maxsize;
	int top;
	double items[16];
}stack_int;

//struct stack_int* newStack(int cap);
int size( stack_int *ns);
int isEmpty( stack_int *ns);
int isFull( stack_int *ns);
void push( stack_int *ns, double x);
double peek( stack_int *ns);
double pop( stack_int *ns);


#endif /* STACK_INT_H_ */