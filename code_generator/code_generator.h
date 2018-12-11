#pragma once

#include "parser.h"


void processSimpleVariable(Node *ptr, int typeSpecifier, int typeQualifier);
void processArrayVariable(Node *ptr, int typeSpecifier, int typeQualifier);

int checkPredefined(Node *ptr);
void processDeclaration(Node *ptr);
void processFuncHeader(Node *ptr);
void processOperator(Node *ptr);
void processStatement(Node *ptr);
void processCondition(Node *ptr);
void processFunction(Node *ptr);

void genLabel(char *label);
void codeGen(Node *ptr);