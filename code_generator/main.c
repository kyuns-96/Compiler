#include <stdio.h>
#include <stdlib.h>
#include "parser.c"
#include "scanner.c"
#include "code_generator.c"

FILE *astFile;
FILE *ucodeFile;

void main (int argc, char *argv[]){
	char fileName[30];
	Node *root;

	printf(" *** start of Mini C Compiler\n");
	if(argc != 2){
		icg_error(1);
		exit(1);
	}

	strcpy(fileName, argv[1]);

	printf("   * source file name : %s\n", fileName);

	if((sourceFile = fopen(fileName , "r")) == NULL){
		icg_error(2);
		exit(1);
	}
	astFile = fopen(strcat(strtok(fileName, "."),".ast"), "w");
	ucodeFile = fopen(strcat(strtok(fileName, "."),".uco"), "w");

	printf(" === start of Parser\n");
	root = parser();
	printTree(root, 0);
	printf(" === start of ICG\n");
	codeGen(root, 0);
	printf(" *** end of Mini C Compiler\n");
} //end of main



