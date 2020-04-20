/*
 * stack_int.c
 *
 * Created: 6/10/2019 3:29:24 PM
 *  Author: Jack Ramirez
 */ 

#include "stack_int.h"
#include <stdlib.h>



// struct stack_int* newStack(int cap){
// 	struct stack_int *ns = (struct stack_int*)malloc(sizeof(struct stack_int));
// 	ns->maxsize = cap;
// 	ns->top = -1;
// 	ns->items = (int*)malloc(sizeof(int) * cap);
// 	return ns;
// }

int size( stack_int *ns){
	return ns->top + 1;
}

int isEmpty( stack_int *ns){
	return ns->top == -1;
}

int isFull( stack_int *ns){
	return ns->top == ns->maxsize - 1;
}

void push( stack_int *ns, double x){
	if(!isFull(ns)){
		ns->items[++ns->top] = x;
	}
}

double peek( stack_int *ns){
	if(!isEmpty(ns)){
		return ns->items[ns->top];
	}
	return -1;
}


double pop( stack_int *ns){
	if(!isEmpty(ns)){
		return ns->items[ns->top--];
	}
}