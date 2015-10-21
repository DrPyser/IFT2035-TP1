#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef struct digit {
  int value;
  struct digit *gch;
  struct digit *drt;
} digit;

typedef struct num {
  int positif;
  digit *first;
  digit *last;
  int refs;
  int length;
} num;

int isPositive(num *number){
    return number && number->positif;
}

int isZero(num *number){
    return (number->first->drt == NULL) && (number->first->value == 0);
}

void compress(num* number){
    if(number != NULL){
	digit *dig = number->first;
	while((dig->drt) != NULL && (dig->value) == 0){
	    number->first = dig->drt;
	    dig = dig->drt;
	    free(dig->gch);
	}
    }
}

digit* makeDigit(int dig, digit *gch, digit *drt){
    digit *new_digit = malloc(sizeof(digit));
    if(new_digit != NULL){
	new_digit->value = dig;
	new_digit->gch = (digit *) gch;
	new_digit->drt = (digit *) drt;
    }
    return new_digit;
}

num* makeNumber(){
    num *number = malloc(sizeof(num));
    if(number != NULL){
	number->positif = true;
	number->refs = 0;
	number->length = 0;
	number->first = NULL;
	number->last = NULL;
    }
    return number;
}

char destroyDigits(num *number){
    if(number != NULL){
	digit *current = number->first;
	digit *temp = current;
	while(current != NULL){
	    current = current->drt;
	    free(temp);
	    temp = current;
	}
    }
    return '\0';
}

char destroyNumber(num *number){
    if(number != NULL){
	destroyDigits(number);
	free(number);
    }
    return '\0';
}

char addDigitRight(num *number, int dig){
    if(number == NULL)
	return 'n';//Code d'erreur: problème de construction du nombre
    if(number->first == NULL){
	if((number->first = number->last = makeDigit(dig,NULL,NULL)) == NULL)
	    return 'm';
	number->length++;
    }
    else if (number->first->value == 0 && number->first->drt){
	number->first->value = dig;
    }
    else{
	if((number->last->drt = makeDigit(dig, number->last, NULL)) == NULL)
	    return 'm';
	number->last = number->last->drt;
	number->length++;
    }
    return '\0';
}

char addDigitLeft(num *number, int dig){
    if(number == NULL)
	return 'n';
    if(dig != 0){
	if(number->first == NULL){
	    if((number->first = number->last = makeDigit(dig,NULL,NULL)) == NULL)
		return 'm';
	}
	else{
	    if((number->first->gch = makeDigit(dig, NULL, number->first)) == NULL)
		return 'm';
	    number->first = number->first->gch;
	}
	number->length++;
    }
    return '\0';
}

char addZeroLeft(num *number){
    if(number == NULL)
	return 'n';
    if(number->first == NULL){
	if ((number->first = number->last = makeDigit(0,NULL,NULL)) == NULL)
	    return 'm';
    }
    else{
	if((number->first->gch = makeDigit(0, NULL, number->first)) == NULL)
	    return 'm';
	number->first = number->first->gch;
    }
    number->length++;
    return '\0';
}

char addZeroRight(num *number){
    if(number == NULL)
	return 'n';
    if(number->first == NULL){
	if((number->first = number->last = makeDigit(0,NULL,NULL)) == NULL)
	    return 'm';
    }
    else{
	if((number->last->drt = makeDigit(0, number->last, NULL)) == NULL)
	    return 'm';
	number->last = number->last->drt;
    }
    number->length++;
    return '\0';
}

num* copyNumber(num* number){
    if(number == NULL)
	return NULL;
    num* copy = makeNumber();
    if(copy != NULL){
	copy->positif = number->positif;
	digit *i;
	for(i = number->first; i != NULL; i = i->drt){
	    if(addDigitRight(copy, i->value) != '\0'){
		copy = NULL;
		break;
	    }
	}
    }
    return copy;
}

num* opposite(num* number){
    if(number == NULL)
	return NULL;
    num* opp = makeNumber();
    if(opp != NULL){
	opp->positif = !(number->positif);
	digit *i;
	for(i = number->first; i != NULL; i = i->drt){
	    if(addDigitRight(opp, (i->value)) != '\0'){
		opp = NULL;
		break;
	    }
	}
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
	dig = dig->drt;
    }
}

int gt(num* a, num* b){
    if((a->length) > (b->length)){
	return true;
    }
    else if((a->length) == (b->length)){
	int stop = false;
	int greater = false;
	digit *i, *j;
	for(i = (a->first), j = (b->first); i != NULL && j != NULL && !stop; i = i->drt, j = j->drt){
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
    if (a == NULL || b == NULL)
	return NULL;
    
    num* sum;
    if (isZero(a)){
	return b;
    }
    else if(isZero(b)){
	return a;
    }
    else if ((a->positif) && !(b->positif)){
	num* opp = opposite(b);
	sum = sub(a,opp);
	destroyNumber(opp);
    }
    else if (!(a->positif) && (b->positif)){
	num* opp = opposite(a);
	sum = sub(b,opp);
	destroyNumber(opp);
    }
    else {
	if (a->length < b->length){
	    return add(b,a);
	} else {
	    sum = makeNumber();
	    if(sum == NULL)
		return sum;
	    int carry = 0;
	    int val1;
	    int val2;
	    int s;
	    digit *i, *j;
	    for(i = a->last, j = b->last; i != NULL; i = i->gch, j = (j != NULL)?(j->gch):j){
		val1 = i->value;
		val2 = (j != NULL)?(j->value):0;
		s = val1 + val2 + carry;
		carry = s / 10;
		s %= 10;
		if(s != 0){
		    if(addDigitLeft(sum, s) != '\0'){
			sum == NULL;
			break;
		    }
		}
		else {
		    if(addZeroLeft(sum) != '\0'){
			sum == NULL;
			break;
		    }
		}
		
	    }
	    if(sum != NULL){
		if (carry != 0)
		    if(addDigitLeft(sum, carry) != '\0')
			sum = NULL;
		if(sum != NULL){
		    compress(sum);
		    sum->positif = (a->positif) && (b->positif);
		}
	    }
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
    if(a == NULL || b == NULL)
	return NULL;
    if(isZero(a)){
	return opposite(b);
    }
    else if(isZero(b)){
	return a;
    }
    else if ((a->positif) && !(b->positif)){
	num* oppb = opposite(b);
	difference = add(a,oppb);
	oppb && destroyNumber(oppb);
    }
    else if (!(a->positif) && (b->positif)){
	num* oppa = opposite(a);
	difference = add(oppa,b);
	if(difference != NULL)
	    difference->positif = false;
	destroyNumber(oppa);
    }
    else if (!(a->positif) && !(b->positif)){
	num* oppb = opposite(b);
	num* oppa = opposite(a);
	difference = sub(oppa, oppb);
	destroyNumber(oppa);
	destroyNumber(oppb);
    }
    else {
	if(gt(b,a)){
	    difference = sub(b,a);
	    difference->positif = false;	    
	}
	else{
	    difference = makeNumber();
	    if(difference == NULL)
		return NULL;
	    int carry = 0;
	    int val1;
	    int val2;
	    digit *i, *j;
	    for(i = a->last, j = b->last; i != NULL; i = i->gch, j = (j != NULL)?(j->gch):j){
		val1 = i->value;
		val2 = (j != NULL)?(j->value):0;
		val2 += carry;
		if(val1 > val2){
		    if(addDigitLeft(difference, val1 - val2) != '\0'){
			difference = NULL;
			break;
		    }
		}
		else if(val1 == val2){
		    if(addZeroLeft(difference) != '\0'){
			difference = NULL;
			break;
		    }
		}
		else {
		    if(addDigitLeft(difference, ((val1 + 10) - val2)) != '\0'){
		       difference = NULL;
		       break;
		    }
		}
		carry = (val1 < val2);
	    }
	    compress(difference);
	}
    }
    putchar('\n');	
    
    return difference;	
}

/*
Implémentation de l'algorithme de la "long multiplication" pour des 'num'.
[in] num* a: un 'num'.
[in] num* b: un 'num'.
[out] num* : le résultat du produit de a et b. 
*/
num* longMul(num* a, num* b){
    if(a == NULL || b == NULL)
	return NULL;
    
    if(isZero(a)){
	return a;
    }
    else if(isZero(b)){
	return b;
    }
    else {
	num *product = makeNumber();
	if(product == NULL)
	    return NULL;
	
	int z;
	for(z = 0; z < a->length + b->length; z++){
	    if(addZeroLeft(product) != '\0')
		return NULL;
	}
	
	//printf("initial product:");
	//printNumber(product);
	//putchar('\n');
	int carry;
	digit *i, *j, *k, *current;
	for(i = b->last, k = product->last; i != NULL; i = i->gch, k = k->gch){
	    for(j = a->last, carry = 0, current = k; j != NULL; j = j->gch, current = current->gch){
		//compute stuff
		current->value += carry + ((i->value) * (j->value));
		carry = (current->value) / 10;
		current->value %= 10;
	    }
	    current->value += carry;
	}
	compress(product);
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
    int d;
    number->positif = (a >= 0);//signe
    a = (a >= 0)? a: -a;//Valeur absolue
    while(a != 0){
	d = a % 10;
	if(d == 0){
	    addZeroLeft(number);
	}
	else {
	    addDigitLeft(number, d);
	}
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
num* stringToNum(char *s){
    num *number = makeNumber();
    if(s[0] == '-'){
	number->positif = false;
    }
    int i;
    for(i = !(number->positif); i < strlen(s); i++){
	addDigitRight(number, (int) s[i] - '0');
    }
    return number;
}


int main(){
    num* a = stringToNum("15241578750190521");
    num* b = stringToNum("15241578750190521");
    num* sum = add(a,b);
    num* product = longMul(a,b);

    printf("a: ");
    printNumber(a);
    putchar('\n');
    printf("b: ");    
    printNumber(b);
    putchar('\n');

    
//    printf("-a: ");
//    printNumber(opposite(a));
//   putchar('\n');
//    printf("-b: ");    
//    printNumber(opposite(b));
//    putchar('\n');

    printf("a + b: ");    
    printNumber(sum);
    putchar('\n');

    printf("a - b: ");    
    printNumber(sub(a,b));
    putchar('\n');

    printf("a * b: ");
    printNumber(product);
    putchar('\n');
/*
//    putchar('\n');    

    destroyNumber(sum);
    destroyNumber(a);
    destroyNumber(b);    

    printf("sum, a et b détruits");
    
    num* c = stringToNum("2520");
    num* d = stringToNum("-125");

    printf("c: ");
    printNumber(c);
    putchar('\n');
    printf("d: ");    
    printNumber(d);
    putchar('\n');
    putchar('\n');

    sum = add(c,d);
    printf("c + d: ");
    printNumber(sum);
    putchar('\n');
*/    

    putchar('\n');    
    printf("a > b: %d\n", gt(a,b));

    return 0;
}

