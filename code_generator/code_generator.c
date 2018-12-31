#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "scanner.c"
#include "parser.c"
#include "SymTab.c"
#include "code_generator.h"

extern FILE *ucodeFile;
int base = 0;
int offest = 0;
int width = 0;
int lvalue = 0;
int rvalue = 0;
int flag = 0;

void processDeclaration(Node *ptr) {
	int typeSpecifier;
	int typeQualifier;
	Node *p, *q;

	if(ptr -> token.number != DCL_SPEC) icg_error(4);

	printf("processDeclaration!\n");
	//step 1 : process DCL_SPEC
	typeSpecifier = INT_TYPE;
	typeQualifier = VAR_TYPE;

	p = ptr -> son;

	while(p) {
		if(p -> token.number == INT_NODE)
			typeSpecifier = INT_TYPE;
		else if(p -> token.number == CONST_NODE)
			typeQualifier = CONST_TYPE;
		else { // AUTO, EXTERN, REGISTER, FLOAT, DOUBLE, SIGNED, UNSIGNED
			printf("not yet implemented!!\n");
			return;
		}
		p = p -> brother;
	}

	//step 2 : process DCL_ITEM
	p = ptr -> brother;

	if(p -> token.number != DCL_ITEM) icg_error(5);

	while(p) {	//SIMPLE_VAR or ARRAY_VAR
		q = p -> son;
		switch(q -> token.number) {
			case SIMPLE_VAR:
				processSimpleVariable(q, typeSpecifier, typeQualifier);
			    break;
			case ARRAY_VAR:
				processArrayVariable(q, typeSpecifier, typeQualifier);
			    break;
			default:
				printf("error in SIMPLE VAR or ARRAY_VAR\n");
			    break;
		} // end switch
		p = p -> brother;
	} // end while
}

void processSimpleVariable(Node *ptr, int typeSpecifier, int typeQualifier) {
	int sign = 1;

	Node *p = ptr -> son;
	Node *q = ptr -> brother;
	int stIndex, size, initialValue;
	size = 1;

	if(ptr -> token.number != SIMPLE_VAR)
		printf("error in SIMPLE_VAR\n");
	
	if(typeQualifier == CONST_TYPE){
		if(q == NULL){
			printf("%s must have a constant value\n", ptr -> token.value.id);
			return;
		}

		if(q -> token.number == UNARY_MINUS){
			sign = -1;
			q = q -> son;
		}

		initialValue = sign * (q -> token.value.num);
		stIndex = insert(p -> token.value.id, typeSpecifier, typeQualifier, 0/* base */, 0/* offset */, width/* width */, initialValue);

	} else {
		stIndex = insert(p -> token.value.id, typeSpecifier, typeQualifier, base, offset, size, 0);
		offset++;
    }
}

void processArrayVariable(Node *ptr, int typeSpecifier, int typeQualifier) {
	Node *p = ptr -> son;
	int stIndex,size;

	if(ptr -> token.number != ARRAY_VAR) {
		printf("error in ARRAY_VAR\n");
		return;
	}

	if(p -> brother == NULL) {
		printf("array size must be typeSpecifierified\n");
		return;
	}
	else
		size = p -> brother -> token.value.num;

	stIndex = insert(p -> token.value.id, typeSpecifier, typeQualifier, base, offset, size, 0);

	offset += size;

}

void processOperator(Node *ptr) {
	int stIndex;
	switch(ptr -> token.number) {
		// assignment operator
		case ASSIGN_OP: 
        {
			Node *lhs = ptr -> son;
			Node *rhs = ptr -> son -> brother;

			if(lhs -> token.number == ASSIGN_OP && rhs -> token.number == ASSIGN_OP)
				flag = 1;

			// step 1 : generate instructions for left-hand side if INDEX node.
			if(lhs -> noderep == nonterm) {
				lvalue = 1;
				processOperator(lhs);
				lvalue = 0;
			}
			// step 2 : generate instructions for right-hand side
			if(rhs -> noderep == nonterm) {
				rvalue = 1;
				processOperator(rhs);
				rvalue = 0;
			}

			else{
				rv_emit(rhs);
			}
			

			//step 3 : generate a store instruction
			if(lhs -> noderep == terminal) {
				stIndex = lookup(lhs -> token.value.id);
				if(stIndex == -1) {
					printf("undefined variable: %s\n", lhs -> token.value.id);
					return;
				}		

				emit2("str",symbolTable[stIndex].base, symbolTable[stIndex].offset);
			} else
				emit0("sti");
			break;
		}
		// complex assignment operators
		case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
		case DIV_ASSIGN: case MOD_ASSIGN: 
        {
			Node *lhs = ptr -> son;
			Node *rhs = ptr -> son -> brother;

			int nodeNumber = ptr -> token.number;

			ptr -> token.number = ASSIGN_OP;

			// step 1 : code generation for left hand side
			if(lhs -> noderep == nonterm) {
				lvalue = 1;
				processOperator(lhs);
				lvalue = 0;
			}

			ptr -> token.number = nodeNumber;

			// step 2 : code generation for repeatng part
			if(lhs -> noderep == nonterm)
				processOperator(lhs);
			else
				rv_emit(lhs);

			// step 3 : code generation for right hand side
			if(rhs -> noderep == nonterm)
				processOperator(rhs);
			else
				rv_emit(rhs);



			// step 4 : emit the correspoinding operation code			
			switch(ptr -> token.number) {
				case ADD_ASSIGN: emit0("add");	break;
				case SUB_ASSIGN: emit0("sub"); 	break;
				case MUL_ASSIGN: emit0("mult"); break;
				case DIV_ASSIGN: emit0("div"); 	break;
				case MOD_ASSIGN: emit0("mod"); 	break;
			}

			// step 5 : code generation for store code
			if(lhs -> noderep == terminal) {
				stIndex = lookup(lhs -> token.value.id);
				if(stIndex == -1)	return;

				emit2("str", symbolTable[stIndex].base , symbolTable[stIndex].offset);
			}
			else
				emit0("sti");
			break;
		}
		// binary(arithmetic / realational / logical) operations
		case ADD:	case SUB:	case MUL:	case DIV:	case MOD:
		case EQ:	case NE:	case GT:	case LT:	case GE:
		case LE:	case LOGICAL_AND: case LOGICAL_OR:
		{
			Node *lhs = ptr -> son;
			Node *rhs = ptr -> son -> brother;

			// step 1 : visit left operand
			if(lhs -> noderep == nonterm)	processOperator(lhs);
			else	rv_emit(lhs);
			// step 2 : visit right operand
			if(rhs -> noderep == nonterm)	processOperator(rhs);
			else 	rv_emit(rhs);

			// step 3 : visit root
			switch(ptr -> token.number) {
				case ADD:		emit0("add");		break;
				case SUB:		emit0("sub");		break;
				case MUL:		emit0("mult");		break;
				case DIV:		emit0("div");		break;
				case MOD:		emit0("mod");		break;
				case EQ:		emit0("eq");		break;
				case NE:		emit0("ne");		break;
				case GT:		emit0("gt");		break;
				case LT:		emit0("lt");		break;
				case GE:		emit0("ge");		break;
				case LE:		emit0("le");		break;
				case LOGICAL_AND:	emit0("and");	break;
				case LOGICAL_OR:	emit0("or");	break;
			}
			if(flag == 1){
				emit0("dup");
				flag = 0;
			}
			break;


		}

		case UNARY_MINUS:	case LOGICAL_NOT:
		{
			Node *p = ptr -> son;

			if(p -> noderep == nonterm)	processOperator(p);
			else 	rv_emit(p);
			
			switch(ptr -> token.number) {
				case UNARY_MINUS:	emit0("neg");	break;
				case LOGICAL_NOT:	emit0("not");	break;
			}
			break;
		}

		case INDEX:
		{
			Node *indexExp = ptr -> son -> brother;

			if(indexExp -> noderep == nonterm)	processOperator(indexExp);
			else 	rv_emit(indexExp);

			stIndex = lookup(ptr -> son -> token.value.id);

			if(stIndex == -1) {
				printf("undefined variable: %s\n", ptr -> son -> token.value.id);
				return;
			}

			emit2("lda", symbolTable[stIndex].base, symbolTable[stIndex].offset);
			emit0("add");

			if(!lvalue) emit0("ldi");
			break;
		}
		// increment / decrement operatiors
		case PRE_INC:	case PRE_DEC:	case POST_INC:	case POST_DEC:
		{
			Node *p = ptr -> son;
			Node *q;
			int stIndex; int amount = 1;

			if(p -> noderep == nonterm)	processOperator(p);
			else 	rv_emit(p);

			q = p;

			while(q -> noderep != terminal)	
				q = q -> son;

			if(!q || (q -> token.number != tident)) {
				printf("increment/decrement operators can not be applied in expression\n");
				return;
			}

			stIndex = lookup(q -> token.value.id);
            if(stIndex == -1) return;

			switch(ptr -> token.number) {
				case PRE_INC:	emit0("inc");	break;
				case PRE_DEC:	emit0("dec");	break;
				case POST_INC:	emit0("inc");	break;
				case POST_DEC:	emit0("dec");	break;
			}

			if(p -> noderep == terminal) {
				stIndex = lookup(p -> token.value.id);
				if(stIndex == -1)	return;
				emit2("str", symbolTable[stIndex].base , symbolTable[stIndex].offset);
			} else if(p -> token.number == INDEX) {
				lvalue = 1;
				processOperator(p);
				lvalue = 0;
				emit0("sti");
			}
            else printf("error increase/decrease operators\n");
			break;
		}

		case CALL:
		{
			Node *p = ptr -> son;
			char *functionName;
			int stIndex;
			int noArguments;

            if(checkPredefined(p)) break;
			
            functionName = p -> token.value.id;
			stIndex = lookup(functionName);
			
			if(stIndex == -1)	break;

			noArguments = symbolTable[stIndex].width;
			
			emit0("ldp");
			p = p -> brother;

			while(p) {
				if(p -> noderep == nonterm)	processOperator(p);
				else 	rv_emit(p);

				noArguments--;
				p = p -> brother;
			}

			if(noArguments > 0) 
				printf("%s: too few actual arguments\n", functionName);
			if(noArguments < 0) 
				printf("%s: too many actual arguments\n", functionName);

			emitJump("call", ptr -> son -> token.value.id);
			break;
		}		
	}
}

void processStatement(Node *ptr) {
	Node *p, *q;
	char *startLabel, *endLabel;

	switch(ptr -> token.number) {
		case COMPOUND_ST:
			p = ptr -> son -> brother;
			p = p -> son;
			while (p) {
				processStatement(p);
				p = p -> brother;
			}
			break;

		case EXP_ST:
			if(ptr -> son != NULL) processOperator(ptr -> son);
			break;

		case RETURN_ST:
			if (ptr -> son != NULL) {
				p = ptr -> son;
				if(p -> noderep == nonterm)
					processOperator(p);
				else
					rv_emit(p);
				emit0("retv");
			}
			else
				emit0("ret");
			break;

		case IF_ST:
		{
			char label[LABEL_SIZE];

			genLabel(label);
			processCondition(ptr -> son);
			emitJump("fjp", label);
			processStatement(ptr -> son -> brother);
			emitLabel(label);
		}
		break;

		case IF_ELSE_ST:
		{
			char label1[LABEL_SIZE], label2[LABEL_SIZE]; 

			genLabel(label1);	genLabel(label2);
			processCondition(ptr -> son);

			emitJump("fjp", label1);
			processStatement(ptr -> son -> brother);
			emitJump("ujp", label2);
			
			emitLabel(label1);
			processStatement(ptr -> son -> brother -> brother);
			emitLabel(label2);
		}
		break;

		case WHILE_ST:
		{
			char label1[LABEL_SIZE], label2[LABEL_SIZE];

			genLabel(label1);	genLabel(label2);
			emitLabel(label1);
			processCondition(ptr -> son);

			emitJump("fjp", label2);
			processStatement(ptr -> son -> brother);
			emitJump("ujp", label1);
			emitLabel(label2);
		}
		break;
	}
}
void processCondition(Node *ptr) {
	if(ptr -> noderep == nonterm)	processOperator(ptr);
	else 	rv_emit(ptr);
}

void processFuncHeader(Node *ptr) {
	int noArguments,returnType;
	int stIndex;
	Node *p = ptr;

	printf("processFunctionHeader\n\n");

	if(ptr -> token.number != FUNC_HEAD){
		printf("error in processFunctionHeader!\n");
		printTree(ptr,0);
		exit(1);
	}

	// step 1 : process the function return type
	p = ptr -> son -> son;
	while(p){
		if(p -> token.number == INT_NODE) 
			returnType = INT_TYPE;
		else if(p -> token.number == VOID_NODE)
			returnType = VOID_TYPE;
		else printf("invalid function return type!\n");

		p = p -> brother;
	}

	// step 2 : count the number of formal parameters
	p = ptr -> son -> brother -> brother;  // FORMAL_PARA
	p = p -> son;  // PARAM_DCL

	noArguments = 0;

	while(p){
		noArguments++;
		p = p -> brother;
	}

	// step 3 : insert the function name
	stIndex = insert(ptr -> son -> brother -> token.value.id, returnType, FUNC_TYPE, 1/*base*/,0/*offset*/,noArguments/*width*/,0/*initalValue*/);

}

void codeGen(Node *ptr) {
	Node *p;
	int globalSize;

	initSymbolTable();
	// step 1 : process the declaration part
	for(p = ptr -> son; p; p = p -> brother){
		if(p -> token.number == DCL){	
			printf("\n\n\n");
			processDeclaration(p -> son);
		}
		else if(p -> token.number == FUNC_DEF) processFuncHeader(p -> son);
		else icg_error(3);
	}

	// dumpSymbolTable();
	globalSize = offset - 1;
	// printf("size of global variables = %d\n", globalSize);

	genSym(base);

	// step 2 : process the function part
	for(p = ptr -> son; p;p = p -> brother)
		if(p -> token.number == FUNC_DEF) processFunction(p);
	//if(!mainExist) warningmsg("main does not exist");

	// step 3 : generate codes for starting routine
	emit1("bgn", globalSize);
	emit0("ldp");
	emitJump("call","main");
	emit0("end");
}

int checkPredefined(Node *ptr) {
	Node *p = ptr;
	char *functionName;
	int noArguments;
	int stIndex;
	functionName = p -> token.value.id;

	if(strcmp(functionName,"read") == 0) {
		noArguments = 1;
		emit0("ldp");
		p = p -> brother;
		while(p) {
			if(p->noderep == nonterm)
				processOperator(p);
			else {
				stIndex = lookup(p -> token.value.id);

				if(stIndex == -1)
					return 0;

				emit2("lda", symbolTable[stIndex].base, symbolTable[stIndex].offset);
			}
			noArguments--;
			p = p -> brother;
		}

		if(noArguments > 0) printf("%s: too few actual arguments\n", functionName);
		else if(noArguments < 0) printf("%s: too many actual arguments\n", functionName);

		emitJump("call", functionName);
		return 1;
	}
	else if(strcmp(functionName, "write") == 0) {
		noArguments = 1;
		emit0("ldp");
		p = p -> brother;
		while(p) {
			if(p -> noderep == nonterm)
				processOperator(p);
			else {
				stIndex = lookup(p -> token.value.id);

				if(stIndex == -1)
					return 0;

				emit2("lod", symbolTable[stIndex].base, symbolTable[stIndex].offset);
			}
			noArguments--;
			p = p -> brother;
		}

		if(noArguments > 0) printf("%s: too few actual arguments\n", functionName);
		else if(noArguments < 0) printf("%s: too many actual arguments\n", functionName);

		emitJump("call", functionName);
		return 1;

	}
	
	return 0;
}


void processFunction(Node *ptr) { 
	Node *p;
	int i;
	
	printf("prcessFunction\n\n");
	printTree(ptr,0);
	// process function header

	// process function body
	// step 1 : process formal parameters
	if(ptr -> son -> son -> brother -> brother -> son) {	// PARAM_DCL
		for(p = ptr -> son -> son -> brother -> brother -> son; p ; p = p -> brother) {
			if(p -> token.number == PARAM_DCL)
				processDeclaration(p -> son);
			else return;
		}
	}

	// step 2 : process the declaration part in function body
	for(p = ptr -> son -> brother -> son -> son; p  ; p = p->brother) {
		if(p->token.number == DCL)
			processDeclaration(p -> son);
		else return;
	}

	// step 3 : emit the funtion start code
	emitFunc("fun", offset-1, base, 2);
 	
	for(i = 0;i < symbolTableTop; i++){
		genSym(i);
	}

	// step 4 : process the statement part in function body	
	for(p = ptr->son; p; p = p->brother) {
		if(p->token.number == COMPOUND_ST)
			processStatement(p);
	}

	// step 5 : check if return check if return type and return value
	emit0("ret");

	// step 6 : generate the ending codes
	emit0("end");
}
