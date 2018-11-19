#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner_main.h"

FILE *fp;

struct tokenType scanner (){
	struct tokenType token; 
	int i,j,k,num;
	char ch,id[ID_LENGTH];

	token.number = tnull;

	do{
		while(isspace(ch = fgetc(fp)));

		if(isalpha(ch)){
			i = 0;
			do{
				if(i < ID_LENGTH)
					id[i++] = ch;

				ch = fgetc(fp);
			} while (isalnum(ch));
			id[i] = '\0';
		
			ungetc(ch, fp);
			
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
				ch = fgetc(fp);
			} while (isdigit(ch));

			ungetc(ch, fp);
			token.number = tnumber;
			token.value.num = num;
		} // number
		
		else{
			switch (ch) {		//special characters
				case '/' : 
					token.value.id[0] = ch; 
					ch = fgetc(fp); // state 10

					if(ch == '*'){
						do {
							while(ch != '*')
								ch = fgetc(fp);

							ch = fgetc(fp);
						} while(ch != '/');

					} else if(ch == '/'){
						while(fgetc(fp) != '\n');


					} else if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tdivAssign;
					} else {
						token.value.id[1] = '\0';
						token.number = tdiv;
						ungetc(ch,fp);
					} break;

				case '!' :
					token.value.id[0] = ch; 
					ch = fgetc(fp);
					if(ch == '='){		//state 
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tnotequ;
					} else {			//state 0
						token.value.id[1] = '\0';
						token.number = tnot;
						ungetc(ch, fp);
					} break;

				case '%' :
					token.value.id[0] = ch;
					ch = fgetc(fp);
					if(ch == '='){	//state 3
						token.value.id[1] = ch; token.value.id[2] = '\0'; 
						token.number = tmodAssign;
					} else {		//state 2
						token.value.id[1] = '\0';
						token.number = tmod;
						ungetc(ch, fp);
					} break;

				case '<' :				//state 11
					token.value.id[0] = ch; 	
					ch = fgetc(fp);
					if (ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';  
						token.number = tlesse;
					} else {
						token.value.id[1] = '\0';
						token.number = tless;
						ungetc(ch, fp); //retract
					} break;

				case '>' :				//state 15
					token.value.id[0] = ch; 
					ch = fgetc(fp);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tgreate;
					} else {
						token.value.id[1] = '\0';
						token.number = tgreat;
						ungetc(ch, fp);
					} break;

				case '=' : 
					token.value.id[0] = ch; 
					ch = fgetc(fp);
					if(ch == '='){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tequal;
					} else {
						token.value.id[1] = '\0';
						token.number = tassign;
						ungetc(ch, fp);
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
					ch = fgetc(fp);
					if(ch == '+'){
						token.value.id[1] = ch; token.value.id[2] = '\0';
						token.number = tinc;
					} else {
						token.value.id[1] = '\0';
						token.number = tplus;
						ungetc(ch, fp);
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
					token.number = tlbracket;
					break;

				case '}' :
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
int main (int argc, char **argv){
	struct tokenType token; 
	
	if (argc != 2) { 
		printf("USAGE: sym filename\n");
		 exit(-1); 
	} 
	if ((fp = fopen(argv[1], "rt")) == NULL) { 
		printf("FILEopen error\n"); 
		exit(-1); 
	} 
	else
		printf("FILEopen Success!\n");

	token = scanner(); 

	while (token.number != teof) { 
		if (token.number == tident) 
			printf("Token-----> %-12s : ( %2d, %7s ) \n", token.value.id,token.number,token.value.id); 

		else if (token.number==tnumber) 
			printf("Token-----> %-12d : ( %2d, %7d ) \n", token.value.num,token.number,token.value.num); 

		else 
			printf("Token-----> %-12s : ( %2d, %7d ) \n",token.value.id,token.number,0); 

		token = scanner();
	}

	fclose(fp);


}

