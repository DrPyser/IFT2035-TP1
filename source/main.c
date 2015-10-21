#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

/**fonction qui separe si l'input est un int un symbole ou un char
*@param (in)_input -> char a tester
*(out)char -> le symbole respectif, 'i' si _input est un int, 'c' si _input est un char,
*'e' si _input est un caractere non reconnu*/

char identify(char _input){
    if ((_input == '=') || (_input == '+') || (_input == '-') || (_input == '*') || (_input == '.') || (_input == ':'))
        return _input;
    if ((_input >= '0') && (_input <= '9'))
        return 'i';
    if ((_input >= 'a') && (_input <= 'z'))
        return 'c';
    if (_input == ' ')
        return ' ';
    return 'e';
}


char processToken(num* buffer, char flag, stack* stk){
    char code;
    switch(flag){
    case ' ':
	code = '\0';
	break;
    case 'i':
	code = push(buffer,stk);
	break;
    case '+':
	if (stk->height >= 2){
	    num* arg1 = pop(stk);
	    num* arg2 = pop(stk);
	    code = push(add(arg2,arg1),stk);
	    killOrLetLive(arg1);
	    killOrLetLive(arg2);	    
	} else {
	    code = 'p';
	}
	break;
    case '-':
	if (stk->height >= 2){
	    num* arg1 = pop(stk);
	    num* arg2 = pop(stk);
	    code = push(sub(arg2,arg1),stk);
	    killOrLetLive(arg1);
	    killOrLetLive(arg2);	    
	} else {
	    code = 'p';
	}
	break;

    case '*':
	if (stk->height >= 2){
	    num* arg1 = pop(stk);
	    num* arg2 = pop(stk);
	    code = push(longMul(arg2,arg1),stk);
	    killOrLetLive(arg1);
	    killOrLetLive(arg2);	    
	} else {
	    code = 'p';
	}
	break;

    case '.':
	if(stk->height >= 1){
	    num* top = pop(stk);
	    printNumber(top);
	    killOrLetLive(top);
	    putchar('\n');
	    putchar('\n');	    
	    code = '\0';
	}
	else
	    code = 'p';
    case ':':
	while(stk->top != NULL)
	    killOrLetLive(pop(stk));
	code = '\0';
	break;
    default:
	code = '\0';
	break;
    }
    return code;
}


/*main fonction du programme*/
int main()
{
    char input = '\0';
    char flag = ' ';
    char erreur = '\0';
    /*erreur est une variable qui enregistre un code d'erreur sur la lecture ou l'execution d'une ligne
    '\0' -> pas d'erreur
    'i' -> lecture d'un caractere inconnu
    'c' -> lecture d'un charactere inattandu
    'm' -> erreur d'allocation mémoire
    'o' -> operateur non-attendu
    'd' -> digit inattendu
    'p' -> erreur de pile
    'u' -> variable non-définie
    'f' -> Erreur fatale
    */
    num* buffer = makeNumber();
    
    stack* stk = createStack();
    num* variables[26];
    if(buffer == NULL || stk == NULL || variables == NULL)
	erreur = 'f';
    int i;
    for(i = 0; i < 26; i++){
	variables[i] = NULL;
    }

    //boucle tant que l'utilisateur de quitte pas avec la commande ';' (ou ctrl-c)
    while (input != ';'){
        putchar('>');
        //boucle sur l'entrée
        while ((erreur == '\0') && ((input = getchar()) != '\n') && (input != ';') ){
            switch (identify(input)){
            case ' ':
		erreur = processToken(buffer,flag,stk);
		if((buffer = makeNumber()) == NULL)
		    erreur = 'm';
		break;
            case '=':
                //detection d'erreur syntaxique
                if (flag != ' '){
                    erreur = 'o';
                    break;
                }
                //code execution ...
                //printf("symbole =");
                //gestion du =
                break;
            case '+':

                //detection d'erreur syntaxique
                if (flag != ' '){
                    erreur = 'o';
                    break;
                }
                //code execution ...
                //printf("symbole +");
                //gestion du +
                break;
            case '-':
                //detection d'erreur syntaxique
                if (flag != ' '){
                    erreur = 'o';
                    break;
                }
                //code execution ...
                //printf("symbole -");
                //gestion du -
                break;
            case '*':
                //detection d'erreur syntaxique
                if (flag != ' '){
                    erreur = 'o';
                }
                //code execution ...
                //printf("symbole *");
                //gestion du *
                break;
	    case '.':
		if(flag != ' '){
		    erreur = 'o';
		}
		break;
	    case ':':
		if(flag != ' '){
		    erreur = 'o';
		}
		break;
            case 'c':
                //detection d'erreur syntaxique
                if ((flag != ' ') && (flag != '=')){
                    erreur = 'c';
                    break;
                }
                //code execution ...
                //printf("variable :\n");
                //putchar(input);
                //gestion d'un char
		if(flag == '='){
		    char next = getchar();
		    if(next != ' ' && next != '\n' ){
			//code d'erreur approprié
			switch(identify(next)){
			case 'c':
			    erreur = 'c';
			    break;
			case 'i':
			    erreur = 'd';
			    break;
			default:
			    erreur = 'o';
			    break;
			}
			break;
		    }
		    else {
			//assignation
			num* old_value = variables[(int) input - 'a'];
			if(old_value != NULL){
			    old_value->refs--;
			    killOrLetLive(old_value);
			}
			variables[(int) input - 'a'] = stk->top->number;
			stk->top->number->refs++;
		    }
		    input = next;
		}
		else if(flag == ' '){
		    if (variables[input - 'a'] != NULL){
			push(variables[input - 'a'], stk);
			variables[input - 'a']->refs++;
		    }
		    else {
			erreur = 'u';
		    }
		}
                break;
            case 'i':
                //detection d'erreur syntaxique
                if ((flag != ' ') && (flag != 'i')){
                    erreur = 'd';
                    break;
                }
                //code execution ...
                //printf("digit :\n");
                //putchar(input);
                //gestion d'un digit
		erreur = addDigitRight(buffer, (int) input - '0');
                break;
            case 'e':
                erreur = 'c';
            default:
                break;
            }

	    if(input == '\n')
		break;
            flag = identify(input);

        }//fin du while de lecture de ligne
	
        if (erreur != '\0')
            while (getchar() != '\n');//vide le input
	else
	    erreur = processToken(buffer,flag,stk);

	if((buffer = makeNumber()) == NULL)
	    erreur = 'm';

        switch(erreur){
        case 'i':
            printf("Erreur : caractere non reconnu");
            break;
        case 'c':
            printf("Erreur : catactere non-attendu");
            break;
        case 'm':
            printf("Erreur : Memoire");
            break;
        case 'o':
            printf("Erreur : operateur non-attendu");
            break;
        case 'd':
            printf("Erreur : digit non-attendu");
            break;
        case 'p':
            printf("Erreur de pile");
            break;
	case 'u':
	    printf("Erreur: variable non-définie.");
	    break;
	case 'n':
	    printf("Erreur: problème de construction d'un nombre.");
	case 'f':
	    printf("Erreur fatale: impossible de continuer. Veuillez Quitter.");
	    break;
        case '\0':
        default:
            break;
        }
	putchar('\n');
        if(erreur != 'f')
	    erreur = '\0';
	flag = ' ';
	printStack(stk);		
        putchar('\n');
        //Output result & error code
    }

    printf("done\n");

    return 0;

}
