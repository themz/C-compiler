#pragma once
#include "Scanner.h"
#include "Symbols.h"

typedef enum {
    PARSE_DEFENITION,
    PARSE_FUNC_ARG_DEF,
    PARSE_FUNC_ARG,
    PARSE_STRUCT,
    PARSE_TYPEDEF,
} parserState;

class Parser
{
private:
	map<OperationType, int> priorityTable;
	map<OperationType, bool> unaryOps;
	map<OperationType, bool> rightAssocOps;
	Scanner scanner_;
	Node *parseFactor(int priority = 0);
	Node *parseArrIndex(Node*);
	Node *parseFuncCall(Node*);
    SymTableStack symStack;
    int unnameCount = 0;
    bool defStruct = false;
    
    void parseTypedef();
    StmtBlock *parseBlock();
    Stmt *parseStmt();
    Stmt *parseIf();
    Stmt *parseFor();
    Stmt *parseWhile();
    Stmt *parseDoWhile();
    Stmt *parseJumpStatement();
    void parseDeclaration(const parserState state = PARSE_DEFENITION);
    void parseDefinition(SymType *type, bool isConst, const parserState state = PARSE_DEFENITION);
    void parseFunction(const bool withBlock);
    SymVar *parseArrayDeclaration(SymType *type, string name,bool isConst, bool inFuncDef = false);
    void parseFunctionDeclaration(SymType *type, string name);
    SymTable *parseFunctionsParams();
    SymType *parseComplexDeclaration(SymType *type);
    string parseStruct(const parserState state = PARSE_DEFENITION);
    string parseName(const parserState state = PARSE_DEFENITION);
    SymTable *parseFuncArg(const bool dec = true);
    SymType *parseType(const parserState state = PARSE_DEFENITION, bool isConst = false);
    void addSym(Symbol *symbol);
    void exception(string msg);
    void parseSemicolon();
public:
	Node *parseExp(int priority = 0);
	void parse();
	Parser(Scanner &scanner);
    void printTable(int deep = 0);
};
