#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "scanner.h"

//already defined in MiniC.tbl
//#define NO_RULES 97
//#define GOAL_RULE (NO_RULES+1)
//#define NO_SYMBOLS 85
//#define NO_STATES 153
#define PS_SIZE 100

void dpush(int,int);
int sp,pstk[PS_SIZE];
void semantic(int);
void parser();

#endif

