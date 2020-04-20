/*
 * stack_char.c
 *
 * Created: 6/10/2019 3:33:42 PM
 *  Author: Jack Ramirez
 */ 
#include "stack_char.h"

// struct stack_char* newStack_c(int cap){
// 	struct stack_char *ns = (struct stack_char*)malloc(sizeof(struct stack_char));
// 	ns->maxsize = cap;
// 	ns->top = -1;
// 	ns->items = (char*)malloc(sizeof(char) * cap);
// 	return ns;
// }

int size_c(stack_char *ns){
	return ns->top + 1;
}

int isEmpty_c( stack_char *ns){
	return ns->top == -1;
}

int isFull_c( stack_char *ns){
	return ns->top == ns->maxsize - 1;
}

void push_c( stack_char *ns, char x){
	if(!isFull(ns)){
		ns->items[++ns->top] = x;
	}
}

char peek_c( stack_char *ns){
	if(!isEmpty(ns)){
		return ns->items[ns->top];
	}
	return '0';
}


char pop_c( stack_char *ns){
	if(!isEmpty(ns)){
		return ns->items[ns->top--];
	}
}