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
} parserState;

typedef enum {
    NONE,
    PARSE_EXP,
    PARSE_EXP_SYM,
    PARSE_LEX,
    COD_GEN,
} comState;

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
    parserState pState = PARSE;
    void parseTypedef();
    StmtBlock *parseBlock(SymTable* table = NULL);
    Stmt *parseStmt();
    Stmt *parseIf();
    Stmt *parseFor();
    Stmt *parseWhile();
    Stmt *parseDoWhile();
    Stmt *parseJumpStatement();
    SymVar* parseDeclarator(SymType *type, bool isConst, const parserState state = PARSE_DEFENITION);
    void parseTypeSpec(const parserState state = PARSE_DEFENITION);
    SymVar* parseDirectDeclarator(SymType *type, bool isConst, const parserState state = PARSE_DEFENITION);
    void parseFunction(const bool withBlock);
    SymType *parseArrayDeclaration(SymType *type);
    void parseFunctionDeclaration(SymType *type, string name);
    SymTable *parseFunctionsParams();
    void parseInitializer(SymVar *var);
    Node *parseInitList();
    SymType *parsePointerDeclaration(SymType *type);
    string parseStruct(const parserState state = PARSE_DEFENITION);
    string parseName(const parserState state = PARSE_DEFENITION);
    //string parseDec(SymType *type);
    SymTable *parseFuncArg(const bool dec = true);
    SymType *parseType(const parserState state = PARSE_DEFENITION, bool isConst = false);
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
