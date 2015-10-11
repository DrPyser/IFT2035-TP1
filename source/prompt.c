#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/utilities.h"

int main(){
    char c;
    boolean error = false;
    boolean close = false;
    boolean first_input = true;
    boolean read_number = false;
    boolean read_variable = false;
    boolean read_operator = false;
    boolean read_quit = false;
    char last_character = '\0';
    //  Num *number;
    while (!close){
	putchar('>');
	c = getchar();
	while(c != '\n' && !close){
	    //On lit un 'token', i.e. jusqu'au prochain espace
	    first_input = true;
	    if(isDigit(c)){
		//printf("%c is a number.\n", c);
		//	read_number = true;
		//	number = makeNumber();
		//	number->positif = true;
		//push(number, &stack);
		//number->refs++;
		read_number = true;
	    } else if(isLetter(c)){
		read_variable = true;
	    }
	    while(c != ' ' && c != '\n' && !close){
		if(read_number){
		    //Si on lit un nombre, on s'attend à lire un chiffre
		    if(isDigit(c)){
			printf("Chiffre: %c\n", c); 
			//	    addDigitRight(number,c + 30);
		    } else{
			printf("Erreur: chiffre(s) suivi de la lettre %c.", c);
			error = true;
			break;
		    }

		    switch(type(input)){
		    case 'i':
		    case 'c':
		    case 's':
		    case '=':
		    case '+':
		    case '-':
		    case '*':
		    }
		} else if(read_variable){
		    //Si on lit une variable,
		    //char next = getchar();
		    if(last_character == '\0'){
			last_character = c;
		    }
		    else if (last_character == 'q' && c == 'u'){
			
		    }
		    if(next == 'u' && read_quit){
			close = true;
			read_variable = false;
		    } else if(next == ' ' || next == '\n'){
			printf("Variable: %c\n", c);
			read_quit = false;
		    } else{
			printf("Erreur: Séquence de lettres interdite: %c%c \n", c, next);
			error = true;
			read_quit = false;
		    }
		    break;
		}
		c = getchar();
		first_input = false;
	    }
	    close = read_quit;
	    if(c != '\n' && !close && !error){
		c = getchar();
		//	if(read_number){
		//	  printf("Number: %s\n", toString(number));
		//	}
	    }
	    read_number = false;
	    read_variable = false;
	}
    }
    printf("Au revoir!\n");
    return 0;
}
