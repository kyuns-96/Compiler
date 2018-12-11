#pragma once
#include <stdio.h>
#include "scanner.h"
//already defined in MiniC.tbl
//#define NO_RULES 97
//#define GOAL_RULE (NO_RULES+1)
//#define NO_SYMBOLS 85
//#define NO_STATES 153
#define PS_SIZE 200

typedef struct nodeType{
	struct tokenType token;
	enum {terminal, nonterm} noderep;
	struct nodeType *son;
	struct nodeType *brother;
} Node;

void dpush(int,int);
int sp,pstk[PS_SIZE];
int symbolStack[PS_SIZE];
int stateStack[PS_SIZE];
Node* valueStack[PS_SIZE];
void semantic(int);
Node *parser();
int meaningfulToken(struct tokenType token);
void printTree(Node *pt, int indent);


Node *buildNode(struct tokenType token);
Node *buildTree(int nodeNumber, int rhsLength);

enum nodeNumber {
		ACTUAL_PARAM,	ADD,			ADD_ASSIGN,		ARRAY_VAR,		ASSIGN_OP,
		BREAK_ST,		CALL,			CASE_EXP,		CASE_LST,		CASE_ST,		
		COMPOUND_ST,	CONST_NODE,		CONTINUE_ST,	DCL,			DCL_ITEM,		
		DCL_LIST,		DCL_SPEC,		DEFAULT_ST,		DIV,			DIV_ASSIGN,		
		EQ,				ERROR,			EXP_ST,			FORMAL_PARA,	FOR_DCL,		
		FOR_EXP,		FOR_INC,		FOR_ST,			FUNC_DEF,		FUNC_HEAD,		
		GE,				GT,				IDENT,			IF_ELSE_ST,		IF_ST,			
		INDEX,			INT_NODE,		LE,				LOGICAL_AND,	LOGICAL_NOT,	
		LOGICAL_OR,		LT,				MOD_ASSIGN,		MUL,			MUL_ASSIGN,		
		NE,				NUMBER,			PARAM_DCL,		POST_DEC,		POST_INC,		
		PRE_DEC,		PRE_INC,		PROGRAM,		MOD,    		RETURN_ST,		
		SIMPLE_VAR,		STAT_LIST,		SUB,			SUB_ASSIGN,		SWITCH_ST,		
		UNARY_MINUS,	VOID_NODE,		WHILE_ST		
};

char *nodeName[]	= {
	"ACTUAL_PARAM",	"ADD",			"ADD_ASSIGN",	"ARRAY_VAR",	"ASSIGN_OP",
	"BREAK_ST",		"CALL",			"CASE_EXP",		"CASE_LST",		"CASE_ST",		
	"COMPOUND_ST",	"CONST_NODE",	"CONTINUE_ST",	"DCL",			"DCL_ITEM",		
	"DCL_LIST",		"DCL_SPEC",		"DEFAULT_ST",	"DIV",			"DIV_ASSIGN",		
	"EQ",			"ERROR",		"EXP_ST",		"FORMAL_PARA",	"FOR_DCL",		
	"FOR_EXP",		"FOR_INC",		"FOR_ST",		"FUNC_DEF",		"FUNC_HEAD",		
	"GE",			"GT",			"IDENT",		"IF_ELSE_ST",	"IF_ST",			
	"INDEX",		"INT_NODE",		"LE",			"LOGICAL_AND",	"LOGICAL_NOT",	
	"LOGICAL_OR",	"LT",			"MOD_ASSIGN",	"MUL",			"MUL_ASSIGN",		
	"NE",			"NUMBER",		"PARAM_DCL",	"POST_DEC",		"POST_INC",		
	"PRE_DEC",		"PRE_INC",		"PROGRAM",		"MOD",  	    "RETURN_ST",		
	"SIMPLE_VAR",	"STAT_LIST",	"SUB",			"SUB_ASSIGN",	"SWITCH_ST",		
	"UNARY_MINUS",	"VOID_NODE",	"WHILE_ST"	
};

int ruleName[] = {
	0, 			PROGRAM, 		0, 				0, 				0,
	0, 			FUNC_DEF, 		FUNC_HEAD,		DCL_SPEC,		0,
	0, 			0, 				0, 				CONST_NODE, 	INT_NODE,
	VOID_NODE, 	0, 				FORMAL_PARA,	0, 				0,
	0, 			0, 				PARAM_DCL, 		COMPOUND_ST,	DCL_LIST,
	DCL_LIST,	0,				0,				DCL,			0,
	0,			DCL_ITEM,		DCL_ITEM,		SIMPLE_VAR,		ARRAY_VAR,
	0,			0,				STAT_LIST,		0,				0,
	0,			0,				0,				0,				0,
	0,			EXP_ST,			0,				0,				IF_ST,
	IF_ELSE_ST,	WHILE_ST,		RETURN_ST,		0,				0,
	ASSIGN_OP,	ADD_ASSIGN,		SUB_ASSIGN,		MUL_ASSIGN,		DIV_ASSIGN,
	MOD_ASSIGN,	0,				LOGICAL_OR,		0,				LOGICAL_AND,
	0,			EQ,				NE,				0,				GT,
	LT,			GE,				LE,				0,				ADD,
	SUB,		0,				MUL,			DIV,			MOD,
	0,			UNARY_MINUS,	LOGICAL_NOT,	PRE_INC,		PRE_DEC,
	0,			INDEX,			CALL,			POST_INC,		POST_DEC,
	0,			0,				ACTUAL_PARAM,	0,				0,
	0,			0,				0
};

