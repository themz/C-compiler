#pragma once
#include "Node.h"
#include "Scanner.h"

class Parser
{
private:
	Scanner scanner_;
	Node* parsTerm(int priority = 0);
	Node* parsFactor(int priority = 0);

public:
	Node* parseExp(int priority = 0);
	void parse();
	Parser(Scanner &scanner);
};
