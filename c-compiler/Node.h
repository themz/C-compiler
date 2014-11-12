#include "Lexeme.h"

class SynNode
{
protected:
public:
};

class Node : public SynNode
{
protected:
	Lexeme* lexeme_;
public:
	friend class Parser;
	Node(): lexeme_(0) {}
	Node(Lexeme* lex): lexeme_(lex) {}
	virtual void print(int deep = 0, bool isTree = true) const = 0;
};

class EmptyNode : public Node
{
public:
	EmptyNode(): Node(0) {}
	void print(int deep = 0, bool isTree = true) const;
};

class OpNode : public Node
{
public:
	OpNode(Lexeme* lex): Node(lex) {}
	virtual void print(int deep = 0, bool isTree = true) {}
};

class UnaryOpNode : public OpNode
{
protected:
	Node* operand;
public:
	UnaryOpNode(Lexeme* op, Node* oper);
	void print(int deep = 0, bool isTree = true) const;
};

class BinaryOpNode : public OpNode
{
protected:
	Node* left;
	Node* right;
public:	
	BinaryOpNode(Lexeme* lex, Node* l, Node* r): OpNode(lex), left(l), right(r){};
    bool haveBranch();
	void print(int deep = 0, bool isTree = true) const;
};

class TernaryOpNode : public BinaryOpNode
{
private:
	Node* condition;
public:
	TernaryOpNode(Lexeme* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c){};
	void print(int deep = 0, bool isTree = true) const;
};

class IntNode : public Node
{
public:	
	IntNode(Lexeme* t): Node(t) {}
	void print(int deep = 0, bool isTree = true) const;
};

class FloatNode : public Node
{
public:	
	FloatNode(Lexeme* t): Node(t) {}
	void print(int deep = 0, bool isTree = true) const;
};

class CharNode : public Node
{
public:	
	CharNode(Lexeme* t): Node(t) {}
	void print(int deep = 0, bool isTree = true) const;
};

class IdentifierNode : public Node
{
public:	
	IdentifierNode(Lexeme* t): Node(t) {}
	void print(int deep = 0, bool isTree = true) const;
};

class FunctionalNode : public Node
{
protected:
	Node* name;
	mutable vector<Node*> args;
	void printArgs(int deep, bool isTree = true) const;
public:
	FunctionalNode(Lexeme* lex, Node* n): Node(lex), name(n), args(0){}
	void addArg(Node* arg);
};

class FuncCallNode : public FunctionalNode
{
private:

public:
	FuncCallNode(Lexeme* t, Node* f): FunctionalNode(t, f){}
	void print(int deep = 0, bool isTree = true) const;
};

class ArrNode : public Node
{
private:
    Node* name;
    mutable vector<Node*> args;
public:
    void printArgs(int deep, bool isTree = true) const;
    ArrNode(Lexeme* t, Node* arr): Node(t),name(arr), args(0){};
    void addArg(Node* arg);
	void print(int deep = 0, bool isTree = true) const;
	bool isModifiableLvalue() const;
	bool isLvalue() const { return true; }
};

class PostfixUnaryOpNode : public UnaryOpNode
{
public:
	PostfixUnaryOpNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper) {}
	void print(int deep = 0, bool isTree = true) const;
};

class TypecastNode : public UnaryOpNode
{
private:
	
public:
	TypecastNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper){}
	void print(int deep = 0, bool isTree = true) const;
};