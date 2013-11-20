#pragma once
#include "Lexeme.h"

class Node
{
protected:
	Lexeme* lexeme_;
public:
	Node(): lexeme_(0) {}
	Node(Lexeme* lex): lexeme_(lex) {}
	virtual void print(int offset= 0) const = 0;
};


