/***************************************************************
 *      Symbol Table Management Routine                        *
 ***************************************************************/
#pragma once
#define HASH_BUCKET_SIZE 97
#define SYMTAB_SIZE 200
#define LEVEL_STACK_SIZE 10
#define LABEL_SIZE 12

extern FILE *ucodeFile;

void initSymbolTable();
int hash(char *);
int lookup(char *);
int insert(char*, int, int, int, int, int, int);
void dumpSymbolTable();
void set();
void reset();

typedef enum {
		NON_SPECIFIER,  VOID_TYPE,  INT_TYPE
} TypeSpecifier;

typedef enum {
		NON_QUALIFIER,	FUNC_TYPE, PARAM_TYPE,  CONST_TYPE,  VAR_TYPE
} TypeQuailfier;

char *typeName[] = {
		"none",   "void",   "int"
};

char *qualifierName[] = {
		"NONE", "FUNC_TYPE",  "PARAM_TYPE",  "CONST_TYPE", "VAR_TYPE"
};

typedef enum {
		ZERO_DIMENSION, ONE_DIMENSION
} Dimension;

struct SymbolEntry {
	char symbolName[ID_LENGTH];
	int typeSpecifier;
	int typeQualifier;
	int base;
	int offset;
	int width;					// size
	int initialValue;			// initial value
	int nextIndex;				// link to next entry.
};
struct SymbolEntry symbolTable[SYMTAB_SIZE];		// symbol table
int symbolTableTop;
int hashBucket[HASH_BUCKET_SIZE];
int levelStack[SYMTAB_SIZE];						// level table
int levelTop;
int base, offset;

void emitSym(int base, int offset, int size) {
	fprintf(ucodeFile, "           sym %d %d %d\n", base, offset, size);
	printf("           sym %d %d %d\n", base, offset, size);
}


void emitJump(char *value, char *label) {
	fprintf(ucodeFile, "           %s %s\n", value, label);
	printf("           %s %s\n", value, label);
}


void emit0(char *value){
	fprintf(ucodeFile,"           %s\n",value);
	printf("           %s\n",value);
}

void emit1(char *value,int p){
	fprintf(ucodeFile,"           %s %d\n",value,p);
	printf("           %s %d\n",value,p);
}

void emit2(char *value,int p,int q){
	fprintf(ucodeFile,"           %s %d %d\n",value,p,q);
	printf("           %s %d %d\n",value,p,q);
}

void emitFunc(char *value,int p,int q,int r){
	fprintf(ucodeFile,"           %s %d %d %d\n",value,p,q,r);
	printf("           %s %d %d %d\n",value,p,q,r);
}

void genLabel(char *label) {
	static int labelNum = 0;
	sprintf(label, "$$%d", labelNum++);
}

void emitLabel(char *label){
	fprintf(ucodeFile,"%-10s nop\n",label);
	printf("%-10s nop\n",label);
}

void icg_error(int err){
	printf("error %d\n",err);
}

void rv_emit(Node *ptr) {
	int stIndex;

	if(ptr  ->  token.number == tnumber)
		emit1("ldc", ptr -> token.value.num);	
	else {
		stIndex = lookup(ptr -> token.value.id);

		if(stIndex == -1) return;
		if(symbolTable[stIndex].typeQualifier == CONST_TYPE) // constant
            emit1("ldc", symbolTable[stIndex].initialValue);
		else if(symbolTable[stIndex].width > 1){ 			// array var
            emit2("lda", symbolTable[stIndex].base, symbolTable[stIndex].offset);
        }
		else 												// simple var
            emit2("lod", symbolTable[stIndex].base, symbolTable[stIndex].offset);
	}
}


/*------------------ TABLE INITIALIZE FUNCTION -----------------------------*/
void initSymbolTable()
{
	int i;

	symbolTableTop = -1;
	levelTop = -1;
	base = 1; offset = 1;

	for (i=0; i < HASH_BUCKET_SIZE; i++)
		hashBucket[i] = -1;

	for (i=0; i < LEVEL_STACK_SIZE; i++)
		levelStack[i] = 0;
}

/*--------------------- HASH FUNCTION --------------------------------------*/
int hash(char *symbolName)
{
	int hashValue;

	for (hashValue=0; *symbolName; symbolName++)
		hashValue += *symbolName;

	return (hashValue % HASH_BUCKET_SIZE);
}


/*------------------ LOOKUP FUNCTION ---------------------------------------*/
int lookup(char *symbol)
{
	int stIndex;

	stIndex = hash(symbol);
	for (stIndex = hashBucket[stIndex];
		 stIndex>=0 && strcmp(symbol, symbolTable[stIndex].symbolName);
		 stIndex=symbolTable[stIndex].nextIndex);
		
	if (stIndex < 0)
		printf("undefined identifier: %s\n", symbol);

	return stIndex;
}

/*-------------------- INSERT SYMBOL FUNCTION ------------------------------*/
int insert(char *symbol, int specifier, int qualifier, int base, int offset,
	   int width, int initialValue)
{
	int hashValue;
	int i;

	hashValue = hash(symbol);
	for (i=hashBucket[hashValue]; i >= 0 && strcmp(symbol, symbolTable[i].symbolName);)
		i = symbolTable[i].nextIndex;

	if ((i >= 0) && (base == symbolTable[i].base)) {	// found
		printf("multiply defined identifier : %s\n", symbol);

		printf("base = %d, symbolTable[i].base = %d\n", base, symbolTable[i].base);
		return -1;
	}

	if (symbolTableTop > SYMTAB_SIZE) {
		printf("symbol table overflow\n");
		dumpSymbolTable();
		exit(1);
	}

	symbolTableTop++;
	strcpy(symbolTable[symbolTableTop].symbolName, symbol);
	symbolTable[symbolTableTop].typeSpecifier = specifier;
	symbolTable[symbolTableTop].typeQualifier = qualifier;
	symbolTable[symbolTableTop].base = base;
	symbolTable[symbolTableTop].offset = offset;
	symbolTable[symbolTableTop].width = width;
	symbolTable[symbolTableTop].initialValue = initialValue;

	symbolTable[symbolTableTop].nextIndex = hashBucket[hashValue];
	hashBucket[hashValue] = symbolTableTop;

	return symbolTableTop;
}


/*------------------ SET BLOCK FUNCTION ------------------------------------*/
void set()
{
	++levelTop;
//	printf("level stack top in set function = %d\n", levelTop);
	if (levelTop > LEVEL_STACK_SIZE)
		printf("error: level stack overflow.\n");

	levelStack[levelTop] = symbolTableTop;
	++base; offset = 1;
}

/*--------------------- RESET BLOCK FUNCTION -------------------------------*/
void reset(void)
{
	int hashValue;
	int i;
	
	i = symbolTableTop;
	symbolTableTop = levelStack[levelTop];
	levelTop--;

	for( ; i>symbolTableTop; i--) {
		hashValue = hash(symbolTable[i].symbolName);
		hashBucket[hashValue] = symbolTable[i].nextIndex;
	}
	base--;

}

/*-------------------   Dump Symbol Table   -----------------------------*/
void dumpSymbolTable()
{
	int i;

	printf(" === Dump Symbol Table ===\n");
	for (i=0; i<=symbolTableTop; i++)
		printf("id = %-12s : \ttype = %s, \tqualifier = %s, \tbase = %d, \toffset = %d \twidth = %d, \tvalue = %d\n",
			    symbolTable[i].symbolName,
			    typeName[symbolTable[i].typeSpecifier],
			    qualifierName[symbolTable[i].typeQualifier],
			    symbolTable[i].base,
			    symbolTable[i].offset,
			    symbolTable[i].width,
			    symbolTable[i].initialValue);
}

/*------------------- generate Symbol Table -----------------------------*/
void genSym(int base)
{
	int stIndex;

//	//fprintf(ucodeFile, "// Information for Symbol Table\n");

	for (stIndex = 0; stIndex <= symbolTableTop; stIndex++) {
		if ((symbolTable[stIndex].typeQualifier == FUNC_TYPE) ||
		    (symbolTable[stIndex].typeQualifier == CONST_TYPE)) continue;

		if (base == symbolTable[stIndex].base)
			emitSym(symbolTable[stIndex].base, symbolTable[stIndex].offset, symbolTable[stIndex].width);
	}
}


