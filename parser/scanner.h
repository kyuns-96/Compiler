#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_KEYWORDS 7
#define ID_LENGTH 12
#define NUMKEYWORD 7

struct tokenType scanner ();
enum tsymbol { 
	tnull = -1,
	/*********************** Operator Symbol **************************/ 
	tnot,		tnotequ,	tmod,		tmodAssign,	tident,		tnumber,
	/*0			1			2			3			4			5	*/
	tand,		tlparen,	trparen,	tmul,		tmulAssign,	tplus,
	/*6			7			8			9			10			11	*/
	tinc,		taddAssign,	tcomma,		tminus,		tdec,		tsubAssign,
	/*12		13			14			15			16			17 	*/
	tdiv,		tdivAssign,	tsemicolon,	tless,		tlesse,		tassign,
	/*18		19			20			21			22			23 	*/
	tequal,		tgreat,		tgreate,	tlbracket,	trbracket,	teof,
	/*24		25			26			27			28			29	*/
	/************************** Word Symbol ***************************/ 
	tconst,		telse,		tif,		tint,		treturn,	tvoid,
	/*30		31			32			33			34			35	*/
	twhile,		tlbrace,	tor,		trbrace		
	/*36		37			38			39	*/
};

struct tokenType {
	int number;
	union {
		char id[ID_LENGTH];
		int num;
	} value;	//token value
};

char *keyword[NO_KEYWORDS] = {
	"const",	"else",		"if",	"int",	"return",	"void",	"while"
};

enum tsymbol tnum[NO_KEYWORDS] = {
	tconst,	telse, tif, tint, treturn, tvoid, twhile
};

#endif
