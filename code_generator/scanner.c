#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

struct tokenType scanner (){
	struct tokenType token; 
	int i,j,k,num;
	char ch,id[ID_LENGTH];

	token.number = tnull;

	do{
		while(isspace(ch = fgetc(stdin)));

		if(isalpha(ch)){
			i = 0;
			do{
				if(i < ID_LENGTH)
					id[i++] = ch;

				ch = fgetc(stdin);
			} while (isalnum(ch));
			id[i] = '\0';
		
			ungetc(ch, stdin);
			
			for(i = 0; i < NUMKEYWORD; i++){
				if(!strcmp(id,keyword[i])){
					j = i;
					break;
				}
				else
					j = -1;
			}
		

			if(j != -1){
				token.number = tnum[j];
				strcpy(token.value.id, id);
			} else {
				token.number = tident;
				strcpy(token.value.id, id);
			}
		}
		
		else if (isdigit(ch)){
			//printf("this is in the isdigit");
			num = 0;
			
			do{
				num = 10 * num + (int)(ch - '0');
				ch = fgetc(stdin);
			} while (isdigit(ch));

			ungetc(ch, stdin);
			token.number = tnumber;
			token.value.num = num;
		} // number
		
		else{
			switch (ch) {		//special characters
				case '/' : 
					token.value.id[0] = ch; 
					ch = fgetc(stdin); // state 10

					if(ch == '*'){
						do {
							while(ch != '*')
								ch = fgetc(stdin);

							ch = fgetc(stdin);
						} while(ch != '/');

					} else if(ch == '/'){
						while(fgetc(stdin) != '\n');


					} else if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tdivAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tdiv;
						ungetc(ch,stdin);
					} break;

				case '!' :
					token.value.id[0] = ch; 
					ch = fgetc(stdin);
					if(ch == '='){		//state 
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tnotequ;
					} else {			//state 0
						token.value.id[1] = '\0';
						token.number = tnot;
						ungetc(ch, stdin);
					} break;

				case '%' :
					token.value.id[0] = ch;
					ch = fgetc(stdin);
					if(ch == '='){	//state 3
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tmodAssign;
					} else {		//state 2
						token.value.id[1] = '\0';
						token.number = tmod;
						ungetc(ch, stdin);
					} break;

				case '<' :				//state 11
					token.value.id[0] = ch; 	
					ch = fgetc(stdin);
					if (ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';  
						token.number = tlesse;
					} else {
						token.value.id[1] = '\0';
						token.number = tless;
						ungetc(ch, stdin); //retract
					} break;

				case '>' :				//state 15
					token.value.id[0] = ch; 
					ch = fgetc(stdin);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tgreate;
					} else {
						token.value.id[1] = '\0';
						token.number = tgreat;
						ungetc(ch, stdin);
					} break;

				case '=' : 
					token.value.id[0] = ch; 
					ch = fgetc(stdin);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tequal;
					} else {
						token.value.id[1] = '\0';
						token.number = tassign;
						ungetc(ch, stdin);
					} break;
				case '&' :
					token.value.id[0] = ch; 
					ch = fgetc(stdin);
					if(ch == '&'){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tand;
					} break;

				case '(' : 
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tlparen;
					break;
				case ')' : 
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = trparen;
					break;

				case '+' : 
					token.value.id[0] = ch; 
					ch = fgetc(stdin);
					if(ch == '+'){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tinc;
					} else {
						token.value.id[1] = '\0';
						token.number = tplus;
						ungetc(ch, stdin);
					} break;
					break;

				case '-' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tminus;
					break;

				case '*' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tmul;
					break;

				case ';' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tsemicolon;
					break;

				case '{' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tlbrace;
					break;

				case '}' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = trbrace;
					break;
				case ',' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tcomma;
					break;

				case EOF : 
					token.number = teof;
					break;
 			}	// switch end
		}

		
	}while (token.number == tnull);
	
	return token;
}
