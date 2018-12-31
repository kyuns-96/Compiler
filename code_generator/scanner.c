#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

extern FILE *sourceFile;

struct tokenType scanner (){
	struct tokenType token; 
	int i,j,k,num;
	char ch,id[ID_LENGTH];

	token.number = tnull;

	do{
		while(isspace(ch = fgetc(sourceFile)));

		if(isalpha(ch)){
			i = 0;
			do{
				if(i < ID_LENGTH)
					id[i++] = ch;

				ch = fgetc(sourceFile);
			} while (isalnum(ch));
			id[i] = '\0';
		
			ungetc(ch, sourceFile);
			
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
				ch = fgetc(sourceFile);
			} while (isdigit(ch));

			ungetc(ch, sourceFile);
			token.number = tnumber;
			token.value.num = num;
		} // number
		
		else{
			switch (ch) {		//special characters
				case '/' : 
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile); // state 10

					if(ch == '*'){
						do {
							while(ch != '*')
								ch = fgetc(sourceFile);

							ch = fgetc(sourceFile);
						} while(ch != '/');

					} else if(ch == '/'){
						while(fgetc(sourceFile) != '\n');


					} else if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tdivAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tdiv;
						ungetc(ch,sourceFile);
					} break;

				case '!' :
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
					if(ch == '='){		//state 
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tnotequ;
					} else {			//state 0
						token.value.id[1] = '\0';
						token.number = tnot;
						ungetc(ch, sourceFile);
					} break;

				case '%' :
					token.value.id[0] = ch;
					ch = fgetc(sourceFile);
					if(ch == '='){	//state 3
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tmodAssign;
					} else {		//state 2
						token.value.id[1] = '\0';
						token.number = tmod;
						ungetc(ch, sourceFile);
					} break;

				case '<' :				//state 11
					token.value.id[0] = ch; 	
					ch = fgetc(sourceFile);
					if (ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';  
						token.number = tlesse;
					} else {
						token.value.id[1] = '\0';
						token.number = tless;
						ungetc(ch, sourceFile); //retract
					} break;

				case '>' :				//state 15
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tgreate;
					} else {
						token.value.id[1] = '\0';
						token.number = tgreat;
						ungetc(ch, sourceFile);
					} break;

				case '=' : 
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tequal;
					} else {
						token.value.id[1] = '\0';
						token.number = tassign;
						ungetc(ch, sourceFile);
					} break;
				case '&' :
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
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
					ch = fgetc(sourceFile);
					if(ch == '+'){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tinc;
					} else if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = taddAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tplus;
						ungetc(ch, sourceFile);
					} break;
					break;

				case '-' :
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
					if(ch == '-'){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tdec;
					} else if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tsubAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tminus;
						ungetc(ch, sourceFile);
					}
					break;

				case '*' :
					token.value.id[0] = ch; 
					ch = fgetc(sourceFile);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tmulAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tmul;
						ungetc(ch, sourceFile);
					}
					break;
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
				case '[' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = tlbracket;
					break;
				case ']' :
					token.value.id[0] = ch; token.value.id[1] = '\0';
					token.number = trbracket;
					break;

				case EOF : 
					token.number = teof;
					break;
 			}	// switch end
		}

		
	}while (token.number == tnull);
	
	return token;
}
