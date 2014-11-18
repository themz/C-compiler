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
    virtual bool isLvalue(){ return false; };
    virtual bool isModifiableLvalue(){ return false; }
    virtual SymType* getType(){ return NULL; }
    virtual void print(int deep = 0, bool isTree = true){};
};

class EmptyNode : public Node
{
public:
    EmptyNode(): Node(0) {}
    void print(int deep = 0, bool isTree = true);
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
    bool isModifiableLvalue();
    bool isLvalue();
    SymType* getType();
    void print(int deep = 0, bool isTree = true);
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
    bool isModifiableLvalue();
    bool isLvalue();
    bool haveBranch();
    void print(int deep = 0, bool isTree = true);
    virtual SymType* getType();
};

class TernaryOpNode : public BinaryOpNode
{
private:
    Node* condition;
public:
    TernaryOpNode(Lexeme* op, Node* c, Node* l, Node* r): BinaryOpNode(op, l, r), condition(c){};
    void print(int deep = 0, bool isTree = true);
};

class IntNode : public Node
{
public:
    IntNode(Lexeme* t): Node(t) {}
    void print(int deep = 0, bool isTree = true);
    virtual SymType* getType();
};

class FloatNode : public Node
{
public:
    FloatNode(Lexeme* t): Node(t) {}
    void print(int deep = 0, bool isTree = true);
    virtual SymType* getType();
};

class CharNode : public Node
{
public:
    CharNode(Lexeme* t): Node(t) {}
    void print(int deep = 0, bool isTree = true);
    virtual SymType* getType();
};

class IdentifierNode : public Node
{
    SymVar* var;
public:
    SymVar *getVar(){return var;};
    void setVar(SymVar *var);
    bool isModifiableLvalue();
    bool isLvalue(){ return true; }
    IdentifierNode(Lexeme* t, SymVar* v): Node(t) { var = v;};
    void print(int deep = 0, bool isTree = true);
    virtual SymType* getType();
};

class FuncCallNode : public Node
{
private:
    Node* name;
    mutable vector<Node*> args;
public:
    FuncCallNode(Lexeme* t, Node* f): Node(t), name(f), args(0) {}
    void print(int deep = 0, bool isTree = true);
    void printArgs(int deep, bool isTree = true);
    void addArg(Node* arg);
};

class ArrNode : public Node
{
private:
    Node* name;
    mutable vector<Node*> index;
    SymType* type;
public:
    void printArgs(int deep, bool isTree = true);
    ArrNode(Lexeme* t, Node* arr, SymType *type): Node(t), name(arr), type(type), index(0){};
    void addIndex(Node* arg);
    void print(int deep = 0, bool isTree = true);
    bool isModifiableLvalue();
    bool isLvalue(){ return true; }
    virtual SymType *getType();
};

class PostfixUnaryOpNode : public UnaryOpNode
{
public:
    PostfixUnaryOpNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper) {}
    bool isLvalue() const { return false; }
    bool isModifiableLvalue() const { return false; }
    void print(int deep = 0, bool isTree = true);
};

class TypecastNode : public UnaryOpNode
{
private:
    
public:
    TypecastNode(Lexeme* op, Node* oper): UnaryOpNode(op, oper){}
    void print(int deep = 0, bool isTree = true);
};

//------------------------- Symbols -------------------------



class Symbol
{
private:
    string name;
public:
    Symbol(const string &name = ""): name(name){};
    string getName(){return name;};
    bool isAnonymousSym(){return (name[1] == '#' || name[0] == '#');};
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
    void printParam(int deep = 0, bool printType = true);
    bool hasAnonymousSym();
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
    virtual bool isLvalue(){ return false; }
    virtual bool isModifiableLvalue(){ return false; }
    virtual SymType *getType(){return NULL;};
    virtual bool canConvertTo(SymType* newType) { return false; }
    virtual void setTable(SymTable *t){};
    virtual bool operator == (SymType* t) { return this == t; }
    void print(int deep = 0, bool printType = true);
};

class SymTypeScalar : public SymType
{
private:
public:
    bool isModifiableLvalue(){ return true; }
    bool isLvalue() { return true; }
    bool canConvertTo(SymType* newType);
    SymTypeScalar(const string &name = ""): SymType(name){}
};

class SymTypeInt : public SymTypeScalar
{
private:
public:
    SymTypeInt(const string &name = "int"): SymTypeScalar(name){};
};

class SymTypeVoid : public SymTypeScalar
{
private:
public:
    SymTypeVoid(const string &name = "void"): SymTypeScalar(name){};
};

class SymTypeFloat : public SymTypeScalar
{
private:
public:
    SymTypeFloat(const string &name = "float"): SymTypeScalar(name){};
};

class SymTypeChar : public SymTypeScalar
{
private:
public:
    SymTypeChar(const string &name = "char"): SymTypeScalar(name){};
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
    virtual void setType(SymType *newType){type = newType;};
    virtual bool isConst(){return cnst;};
    bool operator == (SymType* t);
    bool isModifiableLvalue() { return true; }
    bool isLvalue() { return true; }
    bool canConvertTo(SymType* newType);
    
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
    Node *getSize(){return size;};
    void setSymType(SymType *newType){type = newType;};
    virtual SymType *getType(){return type;};
    virtual bool isArray(){return true;};
    bool operator == (SymType* t);
    bool isLvalue(){return true;};
    bool canConvertTo(SymType *newType);
    SymTypePointer* convertToPointer() { return new SymTypePointer(type);};
};

class SymVar : public Symbol
{
private:
    SymType* type;
    Node* exp;
    bool localVar;
    bool param;
    bool cnst;
public:
    SymVar(const string &name, SymType* type, Node* exp, bool constVar = false, bool param = false, bool localVar = true)
    : Symbol(name), type(type), exp(exp), cnst(constVar), param(param), localVar(localVar){}
    virtual bool isConst(){return cnst;};
    bool isLocal(){return localVar;};
    virtual bool isVar(){return true;};
    void print(int deep = 0, bool printType = true);
    virtual SymType *getType(){return type;};
    virtual void setType(SymType *t){ type = t;};
    void setVal(Node *val){exp = val;};
    bool isParam(){return param;};
};

class SymFunc : public SymType
{
private:
    SymType *retType;
    SymTable *args;
    StmtBlock *body;
public:
    SymFunc(string &name, SymType *retType, SymTable *args, StmtBlock *body):SymType(name), retType(retType), args(args), body(body){};
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