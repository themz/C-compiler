#include "Symbols.h"
#include <iostream>

using namespace std;

static const int N = 3;

UnaryOpNode::UnaryOpNode(Lexeme* op, Node* oper): OpNode(op), operand(oper) 
{

}

void EmptyNode::print(int offset, bool isTree) const
{ 
    cout << string(isTree ? offset * N : 0, ' ') << "<empty>" << (isTree ? "\n" :"");
}

void BinaryOpNode::print(int offset, bool isTree) const
{
    if (left != NULL) {
        left->print(offset + 1, isTree);
    } else {
        cout << string(isTree ? offset * N : 0, ' ') << "";
    }
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
    if (right != NULL) {
        right->print(offset + 1, isTree);
    } else {
        cout << string(isTree ? offset * N : 0, ' ') << "";
    } 
}

bool BinaryOpNode::haveBranch()
{
    return left != NULL && right != NULL;
}

void TernaryOpNode::print(int offset, bool isTree) const
{
	string tab = string(isTree ? offset * N : 1, ' ');
	condition->print(offset + 1, isTree);
	cout << tab << "?" << (isTree ? "\n" :"");
	left->print(offset + 1, isTree);
	cout << tab << ":" << (isTree ? "\n" :"");
	right->print(offset + 1, isTree);

}

void IntNode::print(int offset, bool isTree) const
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<IntegerLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

void FloatNode::print(int offset, bool isTree) const
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<FloatLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

void CharNode::print(int offset, bool isTree) const
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<CharLexeme*>(lexeme_)->getValue() << (isTree ? "\n" :"");
}

void IdentifierNode::print(int offset, bool isTree) const
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<IdentificatorLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

void IdentifierNode::setVar(SymVar *v)
{
    var = v;
}

void UnaryOpNode::print(int offset, bool isTree) const
{
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
	operand->print(offset + 1, isTree);
}

void PostfixUnaryOpNode::print(int offset, bool isTree) const
{
	operand->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << dynamic_cast<OperationLexeme*>(lexeme_)->getValue()<< (isTree ? "\n" :"");
}

void FunctionalNode::printArgs(int offset, bool isTree) const
{
	for (int i = 0; i < (int)args.size(); i++)
	{
		args[i]->print(offset + 1, isTree);
		if (i < (int)args.size() - 1)
			cout << string(isTree ? offset * N : 0, ' ') << ',' << (isTree ? "\n" :"");
	}
}

void FunctionalNode::addArg(Node* arg)
{
	if(arg != NULL)
		args.push_back(arg); 
}

void FuncCallNode::print(int offset, bool isTree) const
{
	name->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "(" << (isTree ? "\n" :"");
	printArgs(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << ")" << (isTree ? "\n" :"");
}

void ArrNode::print(int offset, bool isTree) const
{
    name->print(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "[" << (isTree ? "\n" :"");
	printArgs(offset, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << "]" << (isTree ? "\n" :"");
}

void ArrNode::printArgs(int offset, bool isTree) const
{
    for (int i = 0; i < (int)args.size(); i++)
    {
        args[i]->print(offset + 1, isTree);
        if (i < (int)args.size() - 1)
            cout << string(isTree ? offset * N : 0, ' ') << ',' << (isTree ? "\n" :"");
    }
}
void ArrNode::addArg(Node *arg)
{
    if(arg != NULL)
        args.push_back(arg);
}

void TypecastNode::print(int offset, bool isTree) const
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
	case(T_FLOAT):
		typeName = "float";
		break;
    default:
        break;
	}	
	cout << string(isTree ? offset * N : 0, ' ') << typeName << (isTree ? "\n" :"");
	cout << string(isTree ? offset * N : 0, ' ') << "(" << (isTree ? "\n" :"");
	operand->print(offset + 1, isTree);
	cout << string(isTree ? offset * N : 0, ' ') << ")" << (isTree ? "\n" :"");
}

