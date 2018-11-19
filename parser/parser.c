#include <stdio.h>
#include <stdlib.h>
#include "scanner.c"
#include "MiniC.tbl"
#include "parser.h"

void semantic(int n){
	printf("reduced rule number = %d\n",n);
}

void dpush(int a,int b){
	pstk[++sp] = a;
	pstk[++sp] = b;
}

void parser(){
	extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
	extern int leftSymbol[NO_RULES + 1], rightLength[NO_RULES + 1];
	int entry,ruleNumber,lhs;
	int current_state;
	struct tokenType token;

	sp = 0; pstk[sp] = 0;
	token = scanner();

	while(1){
		current_state = pstk[sp];
		entry = parsingTable[current_state][token.number];
		if(entry > 0) {
			dpush(token.number, entry);
			token = scanner();
		}
		else if(entry < 0) {
			ruleNumber = (-1) * (entry);
			if(ruleNumber == GOAL_RULE){
				printf("*** valid source ***\n");
				return;
			}
		
		semantic(ruleNumber);
		sp = sp - rightLength[ruleNumber] * 2;
		lhs = leftSymbol[ruleNumber];
		current_state = parsingTable[pstk[sp]][lhs];
		dpush(lhs,current_state);
		}
		else {
			printf("=== error in source === \n");
			exit(1);
		}
	}	
}

void main (int argc,char *argv[]){
	printf("start of parser\n");
	parser();
	printf("end of parser\n");

}