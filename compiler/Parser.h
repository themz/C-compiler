#pragma once
#include "Node.h"
#include "Scanner.h"

class Parser
{
private:
	map<OperationType, int> priorityTable;
	map<OperationType, bool> unaryOps;
	map<OperationType, bool> rightAssocOps;
	Scanner scanner_;
	Node* parseTerm(int priority = 0);
	Node* parseFactor(int priority = 0);
	Node* parseArrIndex(Node*);
	Node* parseFuncCall(Node*);
	bool needNext;
public:
	Node* parseExp(int priority = 0, bool next = true);
	void parse();
	Parser(Scanner &scanner);
};
