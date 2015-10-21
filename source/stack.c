#include <stdio.h>
#include <stdlib.h>
#include "numbers.c"

#define true 1
#define false 0
typedef int boolean;

typedef struct cell {
  num *number;
  struct cell *prev;
} cell;

typedef struct stack {
  cell *top;
  int height;
} stack;

void printStack(stack *stk){
  cell *top = stk->top;
  while(top != NULL){
    printNumber(top->number);
    putchar('\n');
    top = top->prev;
  }
}

stack* createStack(){
  stack* stk = malloc(sizeof(stack));
  if(stk != NULL){
    stk->height = 0;
    stk->top = NULL;
  }
  return stk;
}

cell* makeStackCell(num* number, cell* prev){
  cell *new = malloc(sizeof(cell));
  if(new != NULL){
    new->number = number;
    new->prev = prev;
  }
  return new;
}

char push(num *number, stack* stk){
  char code;
  if(number != NULL){
    cell* new = makeStackCell(number,stk->top);
    if(new != NULL){
      stk->top = new;
      number->refs++;
      stk->height++;
      code = '\0';
    }
    else
      code = 'm';
  }
  else
    code = 'p';
  return code;
}

num* pop(stack* stk){
  if(stk->height > 0){
    num *value = stk->top->number;
    cell *new_top = stk->top->prev;
    free(stk->top);
    stk->top = new_top;
    stk->height--;
    value->refs--;
    return value;
  }
  else {
    printf("Erreur: Impossible de dépiler une pile vide.");
    return NULL;
  }
}

void swap(stack *stk){
    num *top = pop(stk);
    num *under = pop(stk);
    push(top,stk);
    push(under,stk);
}

void destroyStack(stack* stk){
    while(stk->top != NULL){
	pop(stk);
    }
    free(stk);
}

/*
int main(){

  
  return 0;
}
*/
