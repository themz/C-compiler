#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "exception.h"
#include "Node.h"

enum{
    Variable,
    Type,
    Function,
} SymbolType;

using namespace std;

//-------------Symbols--------------//
class SymType;
class Stmt;
class StmtBlock;


class Symbol
{
private:
    string name;
public:
    Symbol(const string &name = ""): name(name){};
    string getName(){return name;};
    bool isAnonymousSym(){return name[0] == '#';};
    virtual void print(int deep = 0){};
    virtual bool isType(){return false;};
    virtual bool isVar(){return false;};
    virtual bool isStruct(){return false;};
    virtual bool isFunc(){return false;};
    virtual bool isPointer(){return false;};
    virtual SymType *getType(){return NULL;};
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
    void print(int deep = 0);
    void printTypes(int deep = 0);
    void printVariables(int deep = 0);
    void printFunctions(int deep = 0);
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
    void print(int deep = 0);
    void printTypes(int deep = 0);
    void printVariables(int deep = 0);
    void printFunctions(int deep = 0);
    void push(SymTable* table);
    void pop();
};

class EmptySymbol : public Symbol
{
private:
public:
    void print(){cout << " Empty symbol " << endl;};
};

class SymType : public Symbol
{
private:
public:
    SymType(const string &name = ""): Symbol(name){};
    virtual bool isType(){return true;};
    virtual bool isTypedef(){return false;};
    virtual SymType *getType(){return NULL;};
    void print(int deep = 0);
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
    void print(int deep = 0);
    void setSize(Node *newSize){size = newSize;};
    virtual SymType *getType(){return type;};
    //size_t getSize(){return size;};
};

class SymTypeStruct : public SymType
{
private:
    SymTable *table;
public:
    SymTypeStruct(SymTable *table, const string &name): SymType(name), table(table){};
    virtual bool isStruct(){return true;};
    virtual bool isType(){return true;};
    void print(int deep = 0);
};

class SymTypeDef : public SymType
{
private:
    SymType *type;
public:
    SymTypeDef(SymType *type, const string &name): SymType (name), type(type){};
    void print(int deep = 0);
    virtual SymType *getType(){return type;};
    virtual bool isTypedef(){return true;};
};

class SymTypePointer : public SymType
{
private:
    SymType *type;
public:
    SymTypePointer(SymType *type, const string &name = ""): SymType(name), type(type){};
    void print(int deep = 0);
    virtual bool isPointer(){return true;};
    virtual SymType *getType(){return type;};
};

class SymVar : public Symbol
{
private:
    SymType* type;
    Node* exp;
    bool localVar;
    bool constVar;
public:
    SymVar(const string &name, SymType* type, Node* exp, bool constVar = false, bool localVar = true)
        : Symbol(name), type(type), exp(exp), constVar(constVar), localVar(localVar){}
    bool isConst(){return constVar;};
    bool isLocal(){return localVar;};
    virtual bool isVar(){return true;};
    void print(int deep = 0);
    virtual SymType *getType(){return type;};
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
    void print(int deep = 0);
    int getArgCount(){return args->getSize();};
};

//------------------------stmt

class Stmt
{
private:
public:
    Stmt(){};
    virtual void print(int deep = 0){};
};

class StmtBlock
{
private:
    vector<Stmt*> statements;
    SymTable *table;
public:
    void addStmt(Stmt *statement);
    void setSymTable(SymTable *tbl){table = tbl;};
    StmtBlock(SymTable *table){};
    StmtBlock(){};
    virtual void print(int deep = 0);
};

class SingleStmt : public Stmt
{
private:
    Node* exp;
public:
    SingleStmt(Node* exp): exp(exp) {}
    void print(int deep = 0) const { exp->print(deep); }
};



