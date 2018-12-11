#pragma once
#include <stdio.h>
#include <stdlib.h> 
#include "scanner.c"
#include "MiniC.tbl"
#include "parser.h"

void semantic(int n){
	printf("reduced rule number = %d\n",n);
}

int meaningfulToken(struct tokenType token){
	if((token.number == tident) || (token.number == tnumber))
		return 1;

	else
		return 0;
}

Node *buildNode(struct tokenType token){
	Node *ptr;
	ptr = (Node *)malloc(sizeof(Node));
	if(!ptr){
		printf("malloc error in buildNode()!!!\n");
		exit(1);
	}
	ptr -> token = token;
	ptr -> noderep = terminal;
	ptr -> son = ptr -> brother = 0;
	return ptr;
}

Node *buildTree(int nodeNumber, int rhsLength){
	int i,j,start;
	Node *first, *ptr;
	
	i = sp - rhsLength + 1;
	//step 1 : find a first index with node in value stack
	while(i <= sp && valueStack[i] == NULL) i++;
	
	if(!nodeNumber && i > sp) return NULL;

	start = i;
	//step 2 : linking brother
	while(i <= sp - 1){
		j = i + 1;
		while(j <= sp && valueStack[j] == 0) j++;

		if(j <= sp){
			ptr = valueStack[i];
			while(ptr -> brother) ptr = ptr -> brother;
			ptr -> brother = valueStack[j];
		}
		i = j;
	}
	first = (start > sp) ? 0 : valueStack[start];
	//step 3 : making subtree root and linking son
	if(nodeNumber) {
		ptr = (Node *) malloc(sizeof(Node));
		if(!ptr){
			printf("malloc error in buildTree()\n");
			exit(1);
		}
		ptr -> token.number = nodeNumber;
		ptr -> noderep = nonterm;
		ptr -> son = first;
		ptr -> brother = NULL;
		return ptr;
	}
	else{
		return first;
	}

}

void printNode(Node *pt,int indent){
	//extern FILE * astFile; 
	int i;

	for(i = 1; i < indent; i++)
		printf(" ");

	if(pt -> noderep == terminal){
		if(pt -> token.number == tident)
			printf(" Terminal : %s", pt -> token.value.id);
		else if(pt -> token.number == tnumber)
			printf(" Terminal : %d", pt -> token.value.num);
	}

	else {
		int i;
		i = (int) (pt -> token.number);
		printf(" Nonterminal : %s", nodeName[i]);
	}
	printf("\n");
}

void printTree(Node *pt, int indent){
	Node *p = pt;
	while(p != 0){
		printNode(p,indent);
		if(p -> noderep == nonterm)
			printTree(p -> son, indent + 5);

		p = p -> brother;
	}
}

Node *parser(){
	extern int parsingTable[NO_STATES][NO_SYMBOLS+1];
	extern int leftSymbol[NO_RULES+1],rightLength[NO_RULES+1];
	int entry, ruleNumber, lhs;
	int currentState;
	struct tokenType token;
	Node *ptr;

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
			valueStack[sp] = meaningfulToken(token) ? buildNode(token) : NULL;
			token = scanner();
		}

		else if(entry < 0){
			ruleNumber = -1 * entry;
			if(ruleNumber == GOAL_RULE){
				//printf("*** valid source ***\n");
				return valueStack[sp - 1];
			}
			//semantic(ruleNumber);
			ptr = buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
			valueStack[sp] = ptr;
		}
		else{
			printf("=== error in source === \n");
			exit(1);
		}
	}
}


void main (int argc,char *argv[]){
	printf("start of parser\n");
	printTree(parser(),0);
	printf("end of parser\n");

}