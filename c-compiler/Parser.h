#pragma once
#include "Node.h"
#include "Scanner.h"
#include "Symbols.h"

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
public:
	Node *parseExp(int priority = 0);
    void parseDeclaration(bool inFuncDef = false);
    void parseFunction(const bool withBlock);
    SymVar *parseArrayDeclaration(SymType *type, string name,bool isConst, bool inFuncDef = false);
    void parseFunctions(SymType *type, string name);
    SymType *parseComplexDeclaration(SymType *type);
    void parseStruct();
    void parseTypedef();
    SymTable *parseFuncArg(const bool dec = true);
    SymType *parseType();
	void parse();
    void addSym(Symbol *symbol);
	Parser(Scanner &scanner);
    void printTable(int deep = 0);
    void exception(string msg);
};
