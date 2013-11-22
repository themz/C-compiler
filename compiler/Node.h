#pragma once
#include "Lexeme.h"

class Node
{
protected:
	Lexeme* lexeme_;
public:
	Node(): lexeme_(0) {}
	Node(Lexeme* lex): lexeme_(lex) {}
	//virtual void print(int offset = 0) const = 0;
};

class EmptyNode : public Node
{
public:
	EmptyNode(): Node(0) {}
	//void print(int) const {}
};

class OpNode : public Node
{
protected:
	string opName() const;
public:
	OpNode(Lexeme* lex): Node(lex) {}
	//virtual void print(int) {}
};

class BinaryOpNode : public OpNode
{
protected:
	Node* left;
	Node* right;
public:	
	BinaryOpNode(Lexeme* lex, Node* l, Node* r);
	//void print(int) const;
};

class UnaryOpNode : public OpNode
{
protected:
	Node* operand;
public:
	UnaryOpNode(Lexeme* op, Node* oper);
	//void print(int) const;
};

class PostfixUnaryOpNode : public UnaryOpNode
{
public:
	PostfixUnaryOpNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper) {}
	//void print(int) const;
};


class TernaryOpNode : public BinaryOpNode
{
private:
	Node* condition;
public:
	TernaryOpNode(Lexeme* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c) {}
	//void print(int deep) const;
};

class IntNode : public Node
{
public:	
	IntNode(Lexeme* t): Node(t) {}
	//void print(int) const;
};

class FloatNode : public Node
{
public:	
	FloatNode(Lexeme* t): Node(t) {}
	//void print(int) const;
};

class IdentifierNode : public Node
{
public:	
	IdentifierNode(Lexeme* t): Node(t) {}
	//void print(int) const;
};
