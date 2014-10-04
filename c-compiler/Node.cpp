#include "Node.h"
#include <iostream>

using namespace std;

static const int N = 3;

UnaryOpNode::UnaryOpNode(Lexeme* op, Node* oper): OpNode(op), operand(oper) 
{

}

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

void TernaryOpNode::print(int offset) const 
{
	string tab = string(offset * N, ' ');
	condition->print(offset + 1);
	cout << tab << "?" << endl;
	left->print(offset + 1);
	cout << tab << ":" << endl;
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

void CharNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<CharLexeme*>(lexeme_)->getValue() << endl;
}

void IdentifierNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< endl;
}

void UnaryOpNode::print(int offset) const
{
	cout << string(offset * N, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< endl;
	operand->print(offset + 1);
}

void PostfixUnaryOpNode::print(int offset) const
{
	operand->print(offset);
	cout << string(offset * N, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< endl;
}

void FunctionalNode::printArgs(int offset) const
{
	for (int i = 0; i < (int)args.size(); i++)
	{
		args[i]->print(offset + 1);
		if (i < (int)args.size() - 1)
			cout << string(offset * N, ' ') << ',' << endl;
	}
}

void FunctionalNode::addArg(Node* arg)
{
	if(arg != NULL)
		args.push_back(arg); 
}

void FuncCallNode::print(int offset) const
{
	name->print(offset);
	cout << string(offset * N, ' ') << "(" << endl;
	printArgs(offset);
	cout << string(offset * N, ' ') << ")" << endl;
}

void ArrNode::print(int offset) const 
{
	name->print(offset);
	cout << string(offset * N, ' ') << "[" << endl;
	printArgs(offset);
	cout << string(offset * N, ' ') << "]" << endl;
}

void TypecastNode::print(int offset) const
{
	string typeName = "";
	switch (dynamic_cast<ReservedWordLexeme*>(lexeme_)->getRwType())
	{
	case(T_CHAR):
		typeName = "char";
		break;
	case(T_INT):
		typeName = "int";
		break;
	case(T_DOUBLE):
		typeName = "double";
		break;
	}	
	cout << string(offset * N, ' ') << typeName << endl;
	cout << string(offset * N, ' ') << "(" << endl;
	operand->print(offset + 1);
	cout << string(offset * N, ' ') << ")" << endl;
}

