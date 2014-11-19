#pragma once
#include "Scanner.h"
#include "Symbols.h"

typedef enum {
    PARSE,
    PARSE_DEFENITION,
    PARSE_FUNC_ARG_DEF,
    PARSE_FUNC_ARG,
    PARSE_STRUCT,
    PARSE_TYPEDEF,
    PARSE_CYCLE,
    PARSE_FUNC,
    PARSE_IF,
    PARSE_DOT,
    PARSE_START
} parserState;

typedef enum {
    NONE,
    PARSE_EXP,
    PARSE_EXP_SYM,
    PARSE_LEX,
    COD_GEN,
} comState;

class nameStack;
class stateStack;

class Parser
{
private:
	map<OperationType, int> priorityTable;
	map<OperationType, bool> unaryOps;
	map<OperationType, bool> rightAssocOps;
	Scanner scanner_;
    nameStack *names;
    stateStack *state;
    SymTableStack symStack;
    int unnameCount = 0;
    
	Node *parseFactor(int priority = 0);
	Node *parseArrIndex(Node*);
	Node *parseFuncCall(Node*);
   
    void parseTypedef();
    
    StmtBlock *parseBlock(SymTable* table = NULL);
    Stmt *parseStmt();
    Stmt *parseIf();
    Stmt *parseFor();
    Stmt *parseWhile();
    Stmt *parseDoWhile();
    Stmt *parseJumpStatement();

    SymType *parseDeclarator();
    SymType *parseDirectDeclarator(SymType *type);
    SymType *parseArrayDeclaration(SymType *type);
    SymType *parsePointerDeclaration(SymType *type);
    SymType *parseFunctionDeclaration(SymType *type, bool parseParam = false);
    SymType *hitch(SymType* start, SymType* type);
    SymType *revers(SymType* type);
    SymTable *parseFunctionsParams();
    
    void parseExternalDecl();


    void parseInitializer(SymVar *var);
    Node *parseInitList();

    string parseStruct();
    string parseName();
    
    SymTable *parseFuncArg(const bool dec = true);
    SymType *parseType(bool isConst = false);
    void addSym(Symbol *symbol);
    void exception(string msg, bool cond = true);
    void parseSemicolon();
    Node *parseCondition();

public:
    comState cState = NONE;
	Node *parseExp(int priority = 0);
	void parse();
	Parser(Scanner &scanner);
    void printTable(int deep = 0);
};


//Helper

class nameStack {
    vector<string> names;
public:
    void push(string name);
    string pop();
};

class stateStack {
    vector<parserState> states;
public:
    void push(parserState state);
    parserState pop();
    parserState top();
    bool find(parserState state);
};
