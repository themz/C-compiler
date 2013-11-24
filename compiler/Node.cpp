#include "Node.h"
#include <iostream>

using namespace std;

static const int M = 3;

void EmptyNode::print(int deep) const
{ 
	cout << string(deep * M, ' ') << "<empty expression>" << endl;
}

void BinaryOpNode::print(int deep) const
{
	left->print(deep + 1);
	cout << string(deep * M, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< endl;
	right->print(deep + 1);
}

void IntNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<IntegerLexeme*>(lexeme_)->getValue()<< endl;
}

void DoubleNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<DoubleLexeme*>(lexeme_)->getValue() << endl;
}

void IdentifierNode::print(int deep) const
{
	cout << string(deep * M, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< endl;
}