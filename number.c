#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/utilities.h"

typedef struct digit {
  int value;
  struct digit *pred;
  struct digit *next;
} digit;

typedef struct num {
  boolean positif;
  digit *first;
  digit *last;
  int refs;
  int length;
} num;

boolean isPositive(num *number){
    return number->positif;
}

boolean isZero(num *number){
    return (number->first->value == 0);
}

void compress(num* number){
    digit *dig = number->first;
    while((dig->next) != NULL && (dig->value) == 0){
	number->first = dig->next;
	dig = dig->next;
    }
}

digit* makeDigit(int dig, digit *pred, digit *next){
    digit *new_digit = malloc(sizeof(digit));
    new_digit->value = dig;
    new_digit->pred = (digit *) pred;
    new_digit->next = (digit *) next;
    return new_digit;
}

num* makeNumber(){
    num *number = malloc(sizeof(num));
    number->positif = true;
    number->refs = 0;
    number->length = 0;
    return number;
}

void destroyNumber(num *number){
    free(number);
}

void destroyDigits(num *number){
    digit *dig = number->first->next;
    while(dig != NULL){
	free(dig->pred);
	dig = dig->next;
    }
}

void addDigitRight(num *number, int dig){
    if(number->first == NULL){
	number->first = number->last = makeDigit(dig,NULL,NULL);
	number->length++;
    }
    else if (number->first->value == 0 && number->first->next == NULL){
	number->first->value = dig;
    }
    else{
	number->last->next = makeDigit(dig, number->last, NULL);
	number->last = number->last->next;
	number->length++;
    }
}

void addDigitLeft(num *number, int dig){
    if(dig != 0){
	if(number->first == NULL){
	    number->first = number->last = makeDigit(dig,NULL,NULL);
	}
	else{
	    number->first->pred = makeDigit(dig, NULL, number->first);
	    number->first = number->first->pred;
	}
	number->length++;
    }
}

void addZeroLeft(num *number){
    if(number->first == NULL){
	number->first = number->last = makeDigit(0,NULL,NULL);
    }
    else{
	number->first->pred = makeDigit(0, NULL, number->first);
	number->first = number->first->pred;
    }
    number->length++;
}

void addZeroRight(num *number){
    if(number->first == NULL){
	number->first = number->last = makeDigit(0,NULL,NULL);
    }
    else{
	number->last->next = makeDigit(0, number->last, NULL);
	number->last = number->last->next;
    }
    number->length++;
    
}

num* copyNumber(num* number){
    num* copy = makeNumber();
    copy->positif = number->positif;
    for(digit *i = number->first; i != NULL; i = i->next){
	addDigitRight(copy, i->value);
    }
    return copy;
}

num* opposite(num* number){
    num* opp = makeNumber();
    opp->positif = !(number->positif);
    for(digit *i = number->first; i != NULL; i = i->next){
	addDigitRight(opp, i->value);
    }
    return opp;
}


void killOrLetLive(num *number){
    if(number->refs <= 0){
	destroyNumber(number);
    }
}

void printNumber(num *number){
    digit *dig = number->first;
    if(!number->positif){
	putchar('-');
    }
    while(dig != NULL){
	printf("%d",dig->value);
	dig = dig->next;
    }
}


boolean gt(num* a, num* b){
    if((a->length) > (b->length)){
	return true;
    }
    else if((a->length) == (b->length)){
	boolean stop = false;
	boolean greater = false;
	for(digit *i = (a->first), *j = (b->first); i != NULL && j != NULL && !stop; i = i->next, j = j->next){
	    greater = (i->value) > (j->value);
	    stop = greater || (i->value) < (j->value);
	}
	return greater;
    }
    else{
	return false;
    }
}

num* sub(num* a, num* b);

num* add(num* a, num* b){
    printNumber(a);
    printf(" + ");
    printNumber(b);
    putchar('\n');
    
    num* sum;
    if(isZero(a)){
	return b;
    }
    else if(isZero(b)){
	return a;
    }
    else if ((a->positif) && !(b->positif)){
	num* opp = opposite(b);
	sum = sub(a,opp);
	free(opp);
    }
    else if (!(a->positif) && (b->positif)){
	num* opp = opposite(a);
	printf("-a = ");
	printNumber(opp);
	putchar('\n');
	sum = sub(b,opp);
	free(opp);
    }
    else {
	if (a->length < b->length){
	    return add(b,a);
	} else {
	    sum = makeNumber();
	    int carry = 0;
	    int val1;
	    int val2;
	    int s;
	    for(digit *i = a->last, *j = b->last; i != NULL; i = i->pred, j = (j != NULL)?(j->pred):j){
		val1 = i->value;
		val2 = (j != NULL)?(j->value):0;
		s = val1 + val2 + carry;
		if(s != 0){
		    addDigitLeft(sum, s % 10);
		}
		else {
		    addZeroLeft(sum);
		}
		carry = s / 10;
	    }
	    compress(sum);
	    sum->positif = a->positif && b->positif;
	}
    }
    
    return sum;
}

num* sub(num* a, num* b){
    printNumber(a);
    printf(" - ");
    printNumber(b);
    putchar('\n');

    num* difference;
    
    if(isZero(a)){
	return opposite(b);
    }
    else if(isZero(b)){
	return a;
    }
    else if ((a->positif) && !(b->positif)){
	num* opp = opposite(b);
	difference = add(a,opp);
	free(opp);
    }
    else if (!(a->positif) && (b->positif)){
	num* opp = opposite(b);
	difference = add(a,opp);
	free(opp);
    }
    else if (!(a->positif) && !(b->positif)){
	num* opp1 = opposite(b);
	num* opp2 = opposite(a);
	difference = sub(opp1, opp2);
	free(opp1);
	free(opp2);
    }
    else {
	if(gt(b,a)){
	    difference = sub(b,a);
	    difference->positif = false;	    
	}
	else{
	    difference = makeNumber();
	    int carry = 0;
	    int val1;
	    int val2;
	    for(digit *i = a->last, *j = b->last; i != NULL; i = i->pred, j = (j != NULL)?(j->pred):j){
		val1 = i->value;
		val2 = (j != NULL)?(j->value):0;
		val2 += carry;
		if(val1 > val2){
		    addDigitLeft(difference, val1 - val2);
		}
		else if(val1 == val2){
		    addZeroLeft(difference);
		}
		else {
		    addDigitLeft(difference, ((val1 + 10) - val2));
		}
		carry = (val1 < val2);
	    }
	    compress(difference);
	}
    }
    return difference;	
}

/*
Implémentation de l'algorithme de la "long multiplication" pour des 'num'.
[in] num* a: un 'num'.
[in] num* b: un 'num'.
[out] num* : le résultat du produit de a et b. 
*/
num* longMul(num* a, num* b){
    if(isZero(a)){
	return a;
    }
    else if(isZero(b)){
	return b;
    }
    else {
	num *product = makeNumber();
	for(int i = 0; i < (a->length + b->length -1); i++){
	    addZeroLeft(product);
	}
	int carry;
	digit *k = product->last;
	for(digit *i = b->last; i != NULL; i = i->pred){
	    carry = 0;
	    digit *current = k;
	    for(digit *j = a->last; j != NULL; j = j->pred){
		//compute stuff
		current->value += carry + (i->value) * (j->value);
		carry = current->value / 10;
		current->value = current->value % 10;
		current = current->pred;
	    }
	    k = k->pred;
	}
	product->positif = a->positif == b->positif;
	return product;
    }
    
}

/*
Cette fonction génère un num à partir d'un int.
[in] int a: un int.
[out] num* : un pointeur vers le num généré.
 */
num* intToNum(int a){
    num *number = makeNumber();
    number->positif = (a >= 0);//signe
    a = (a >= 0)? a: -a;//Valeur absolue
    while(a != 0){
	addDigitLeft(number,a % 10);
	a /= 10;
    }
    return number;
}

/*
Cette fonction génère un num à partir d'un string représentant un nombre entier.
[in] char* a: un pointeur vers un string représentant un nombre entier. 
Le premier charactère peut optionellement être le symbole '-', auquel cas le nombre sera négatif.
Tout autre charactère dans le string causera un erreur.
[out] num* : un pointeur vers le num généré.
 */
num* stringToNum(char *a){
    num *number = makeNumber();
    if(a[0] == '-'){
	number->positif = false;
    }
    for(int i = !(number->positif); i < strlen(a); i++){
	addDigitRight(number, a[i] - '0');
    }
    return number;
}

int main(){
    
    num* x = stringToNum("-000000000010000012346789934567890");
    printf("x: ");
    printNumber(x);
    putchar('\n');

    num* a = stringToNum("-25");
    num* b = stringToNum("10");
    num* product = longMul(a,b);

    printf("a: ");
    printNumber(a);
    putchar('\n');
    printf("b: ");    
    printNumber(b);
    putchar('\n');
    
    printf("-a: ");
    printNumber(opposite(a));
    putchar('\n');
    printf("-b: ");    
    printNumber(opposite(b));
    putchar('\n');

    printf("a + b: ");    
    printNumber(add(a,b));
    putchar('\n');

    printf("a: ");
    printNumber(a);
    putchar('\n');
    printf("b: ");    
    printNumber(b);
    putchar('\n');

    
    printf("a - b: ");    
    printNumber(sub(a,b));
    putchar('\n');

    printf("a * b: ");
    printNumber(product);
    putchar('\n');
    printf("a > b: %d\n", gt(a,b));


    return 0;
}
