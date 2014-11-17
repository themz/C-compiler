#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "exception.h"
#include "Lexeme.h"

using namespace std;

class SymType;
class Stmt;
class StmtBlock;
class SymTable;
class SymVar;

//------------------------- Expression node -------------------------

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
    Node *getRight(){return right;};
    Node *getLeft(){return left;};
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
    SymVar* var;
public:
    SymVar *getVar(){return var;};
    void setVar(SymVar *var);
    IdentifierNode(Lexeme* t, SymVar* v): Node(t) { var = v;};
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

//------------------------- Symbols -------------------------



class Symbol
{
private:
    string name;
public:
    Symbol(const string &name = ""): name(name){};
    string getName(){return name;};
    bool isAnonymousSym(){return name[1] == '#';};
    virtual void print(int deep = 0, bool printType = true){};
    virtual bool isType(){return false;};
    virtual bool isVar(){return false;};
    virtual bool isStruct(){return false;};
    virtual bool isFunc(){return false;};
    virtual bool isPointer(){return false;};
    virtual bool isArray(){return false;};
    virtual bool isEmpty(){return false;};
    virtual bool isConst(){return false;};
    virtual SymType *getType(){return NULL;};
    virtual void setType(){};
};

//-------------SymTable--------------//

class SymTable
{
private:
    vector<Symbol *> table;
    int size;
    int shift;
    bool hType = false, hVar = false, hFunc = false;
public:
    SymTable(): table(), size(0), shift(0){};
    friend class Parser;
    bool add(Symbol *symbol);
    bool hasType(){return hType;};
    bool hasVar(){return hVar;};
    bool hasFunc(){return hFunc;};
    void print(int deep = 0, bool printType = true);
    void printTypes(int deep = 0, bool printType = true);
    void printVariables(int deep = 0, bool printType = true);
    void printFunctions(int deep = 0, bool printType = true);
    int getSize(){return size;};
    Symbol *top();
    Symbol *find(const string &name);
};

class SymTableStack
{
private:
    vector<SymTable *> tables;
public:
    Symbol *find(const string &name);
    SymTable* top();
    bool existsInLastNamespace(const string& name);
    void add(Symbol* s);
    void print(int deep = 0, bool printType = true);
    void printTypes(int deep = 0, bool printType = true);
    void printVariables(int deep = 0, bool printType = true);
    void printFunctions(int deep = 0, bool printType = true);
    void push(SymTable* table);
    void pop();
};

class EmptySymbol : public Symbol
{
private:
public:
    void print(int deep = 0, bool printType = true){cout << " Empty symbol " << endl;};
};

class SymType : public Symbol
{
private:
public:
    SymType(const string &name = ""): Symbol(name){};
    virtual bool isType(){return true;};
    virtual bool isTypedef(){return false;};
    virtual bool isEmpty(){return false;}
    virtual SymType *getType(){return NULL;};
    virtual void setTable(SymTable *t){};
    void print(int deep = 0, bool printType = true);
};

class SymTypeInt : public SymType
{
private:
public:
    SymTypeInt(const string &name = "int"): SymType(name){};
};

class SymTypeVoid : public SymType
{
private:
public:
    SymTypeVoid(const string &name = "void"): SymType(name){};
};

class SymTypeFloat : public SymType
{
private:
public:
    SymTypeFloat(const string &name = "float"): SymType(name){};
};

class SymTypeChar : public SymType
{
private:
public:
    SymTypeChar(const string &name = "char"): SymType(name){};
};

class SymTypeArray: public SymType
{
private:
    Node *size;
    SymType *type;
public:
    SymTypeArray(Node *size , SymType *type, const string &name = "array"): SymType(name), type(type), size(size){};
    void print(int deep = 0, bool printType = true);
    void setSize(Node *newSize){size = newSize;};
    virtual SymType *getType(){return type;};
    virtual bool isArray(){return true;};
    //size_t getSize(){return size;};
};

class SymTypeStruct : public SymType
{
private:
    SymTable *table;
public:
    SymTypeStruct(SymTable *table, const string &name): SymType(name), table(table){};
    Symbol *find(const string &name);
    virtual bool isStruct(){return true;};
    virtual bool isType(){return true;};
    void print(int deep = 0, bool printType = true);
    virtual void setTable(SymTable *t){ table = t;}
    virtual bool isEmpty(){return table == NULL;}
};

class SymTypeDef : public SymType
{
private:
    SymType *type;
public:
    SymTypeDef(SymType *type, const string &name): SymType (name), type(type){};
    void print(int deep = 0, bool printType = true);
    virtual SymType *getType(){return type;};
    virtual bool isTypedef(){return true;};
};

class SymTypePointer : public SymType
{
private:
    Symbol *type;
    bool cnst;
public:
    SymTypePointer(SymType *type, const bool isConst = false, const string &name = ""): SymType(name), type(type), cnst(isConst){};
    void print(int deep = 0, bool printType = true);
    virtual bool isPointer(){return true;};
    virtual SymType *getType(){return (SymType*)type;};
    virtual bool isConst(){return cnst;};
};

class SymVar : public Symbol
{
private:
    SymType* type;
    Node* exp;
    bool localVar;
    bool cnst;
public:
    SymVar(const string &name, SymType* type, Node* exp, bool constVar = false, bool localVar = true)
    : Symbol(name), type(type), exp(exp), cnst(constVar), localVar(localVar){}
    virtual bool isConst(){return cnst;};
    bool isLocal(){return localVar;};
    virtual bool isVar(){return true;};
    void print(int deep = 0, bool printType = true);
    virtual SymType *getType(){return type;};
    virtual void setType(SymType *t){ type = t;};
    void setVal(Node *val){exp = val;};
};

class SymFunc : public Symbol
{
private:
    SymType *retType;
    SymTable *args;
    StmtBlock *body;
public:
    SymFunc(string &name, SymType *retType, SymTable *args, StmtBlock *body):Symbol(name), retType(retType), args(args), body(body){};
    virtual bool isFunc(){return true;};
    void print(int deep = 0, bool printType = true);
    int getArgCount(){return args->getSize();};
};

//------------------------ Statments -------------------------

class Stmt
{
private:
public:
    Stmt(){};
    virtual void print(int deep = 0, bool printType = true){};
    virtual bool isSingle(){return false;};
};

class StmtBlock : public Stmt
{
private:
    vector<Stmt*> statements;
    SymTable *table;
public:
    void addStmt(Stmt *statement);
    void setSymTable(SymTable *tbl){table = tbl;};
    StmtBlock(SymTable *table){};
    StmtBlock(){};
    virtual void print(int deep = 0, bool printType = true);
};

class StmtSingle : public Stmt
{
private:
    Node* exp;
public:
    StmtSingle(Node* exp): exp(exp) {}
    void print(int deep = 0, bool printType = true);
    bool isSingle(){return true;};
};

class StmtIf : public Stmt
{
private:
    Node *condition;
    Stmt *trueBlock;
    Stmt *falseBlock;
public:
    StmtIf(Node *cond, Stmt *tBlock, Stmt *fBlock): condition(cond), trueBlock(tBlock), falseBlock(fBlock) {};
    void print(int deep = 0, bool printType = true);
};

class StmtCycle : public Stmt
{
protected:
    Node *condition;
    Stmt* body;
public:
    StmtCycle(Node *cond, Stmt* b): condition(cond), body(b) {}
    virtual void print(int deep = 0, bool printType = true){};
};

class StmtWhile : public StmtCycle
{
public:
    StmtWhile(Node *cond, Stmt *b): StmtCycle(cond, b) {}
    void print(int deep = 0, bool printType = true);
};

class StmtDoWhile : public StmtCycle
{
public:
    StmtDoWhile(Node *cond, Stmt *b): StmtCycle(cond, b) {}
    void print(int deep = 0, bool printType = true);
};

class StmtFor : public StmtCycle
{
private:
    Node *init;
    Node *inc;
public:
    StmtFor(Node *init, Node *cond, Node *inc, Stmt *body): StmtCycle(cond, body), init(init), inc(inc) {}
    void print(int deep = 0, bool printType = true);
};

class StmtJump : public Stmt
{
};

class StmtBreak : public StmtJump
{
public:
    void print(int deep = 0, bool printType = true);
};

class StmtContinue : public StmtJump
{
public:
    void print(int deep = 0, bool printType = true);
};

class StmtReturn : public StmtJump
{
private:
    Node *retArg;
public:
    StmtReturn(Node *ret): retArg(ret) {}
    void print(int deep = 0, bool printType = true);
};