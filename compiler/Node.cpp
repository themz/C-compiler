#include "Node.h"
#include <iostream>

using namespace std;

static const int N = 2;

void EmptyNode::print(int offset) const
{ 
	cout << string(offset * N, ' ') << "<empty>" << endl;
}

void BinaryOpNode::print(int offset) const
{
	left->print(offset + 1);
	cout << string(offset * N, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< endl;
	right->print(offset + 1);
}

void IntNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<IntegerLexeme*>(lexeme_)->getValue()<< endl;
}

void DoubleNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<DoubleLexeme*>(lexeme_)->getValue() << endl;
}

void IdentifierNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< endl;
}