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
	Node *parseTerm(int priority = 0);
	Node *parseFactor(int priority = 0);
	Node *parseArrIndex(Node*);
	Node *parseFuncCall(Node*);
    SymTableStack symStack;
public:
	Node *parseExp(int priority = 0);
    void parseDeclaration();
    void parseFunction(const bool withBlock);
    void parseArrayDeclaration(SymType *type, string name,bool isConst);
    void parseComplexDeclaration();
    SymTable *parseFuncArg(const bool dec = true);
    SymType *parseType();
    
	void parse();
	Parser(Scanner &scanner);
    void printTable();
};
