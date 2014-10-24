#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Statement.h"

class Node;

enum{
    Variable,
    Type,
    Function,
} SymbolType;

using namespace std;

//-------------Symbols--------------//

class Symbol
{
private:
    string name;
public:
    Symbol(const string &name = ""): name(name){};
    string getName(){return name;};
    virtual void print(){};
    virtual bool isType(){return false;};
    virtual bool isVar(){return false;};
    virtual bool isFunc(){return false;};
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
    void print();
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
    //size_t getSize(){return size;};
};

class SymTypeStruct : public SymType
{
private:
public:
};

class SymTypeDef : public SymType
{
private:
public:
};

class SymTypePointer : public SymType
{
private:
public:
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
    void print();
};

class SymTable;

class SymFunc : public Symbol
{
private:
    SymType *retType;
    SymTable *args;
    StmtBlock *body;
public:
    virtual bool isFunc(){return true;};
    void print();
};

//-------------SymTable--------------//

class SymTable
{
private:
    vector<Symbol *> table;
    int size;
    int shift;
public:
    friend class Parser;
    bool add(Symbol *symbol);
    void print();
    void printTypes();
    void printVariables();
    void printFunctions();
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
    void print();
    void printTypes();
    void printVariables();
    void printFunctions();
    void push(SymTable* table);
    void pop();
};




