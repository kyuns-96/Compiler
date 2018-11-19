#include "MiniC.tbl"
#include "scanner.c"

#define PS_SIZE 200

void semantic(int);

int errcnt = 0;
int sp;
int stateStack[PS_SIZE];
int symbolStack[PS_SIZE];

void parser(){
	extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
	extern int leftSymbol[NO_RULES+1],rightLength[NO_RULES+1];
	int entry, ruleNumber, lhs;

	int currentState;
	struct tokenType token;

	sp = 0; stateStack[sp] = 0;

	token = scanner();

	while(1){
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];
		if(entry > 0){
			sp++;
			if(sp > PS_SIZE){
				printf("critical compiler error : stack over flow\n");
				exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			token = scanner();
		}

		else if(entry < 0){
			ruleNumber = -1 * entry;
			if(ruleNumber == GOAL_RULE){
				printf("*** valid source ***\n");
				return;
			}
			semantic(ruleNumber);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
		}
		else{
			printf("=== error in source === \n");
			exit(1);
		}
	}
}

void semantic(int n){
	printf("reduced rule number = %d\n",n);
}

void main (int argc,char *argv[]){
	printf("start of parser\n");
	parser();
	printf("end of parser\n");

}