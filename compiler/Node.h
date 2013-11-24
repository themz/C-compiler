#pragma once
#include "Lexeme.h"

class Node
{
protected:
	Lexeme* lexeme_;
public:
	friend class Parser;
	Node(): lexeme_(0) {}
	Node(Lexeme* lex): lexeme_(lex) {}
	virtual void print(int deep = 0) const = 0;
};

class EmptyNode : public Node
{
public:
	EmptyNode(): Node(0) {}
	void print(int) const; 
};

class OpNode : public Node
{
public:
	OpNode(Lexeme* lex): Node(lex) {}
	virtual void print(int deep) {}
};

class BinaryOpNode : public OpNode
{
protected:
	Node* left;
	Node* right;
public:	
	BinaryOpNode(Lexeme* lex, Node* l, Node* r): OpNode(lex), left(l), right(r){};
	void print(int) const;
};

//class UnaryOpNode : public OpNode
//{
//protected:
//	Node* operand;
//public:
//	UnaryOpNode(Lexeme* op, Node* oper);
//	void print(int) const{ cout << "unary";};
//};
//
//class PostfixUnaryOpNode : public UnaryOpNode
//{
//public:
//	PostfixUnaryOpNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper) {}
//	void print(int) const;
//};


class TernaryOpNode : public BinaryOpNode
{
private:
	Node* condition;
public:
	TernaryOpNode(Lexeme* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c) {}
	void print(int deep) const;
};

class IntNode : public Node
{
public:	
	IntNode(Lexeme* t): Node(t) {}
	void print(int) const;
};

class DoubleNode : public Node
{
public:	
	DoubleNode(Lexeme* t): Node(t) {}
	void print(int) const;
};

class IdentifierNode : public Node
{
public:	
	IdentifierNode(Lexeme* t): Node(t) {}
	void print(int) const;
};
