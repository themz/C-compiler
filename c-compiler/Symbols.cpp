#include "Symbols.h"
#include <map>


#define N 3

//--------------------------------SymTable--------------------------------//

bool SymTable::add(Symbol *symbol)
{
    Symbol *s = find(symbol->getName());
    if (s != NULL) {
        return false;
    }
    table.push_back(symbol);
    hFunc = max(symbol->isFunc(), hFunc);
    hVar = max(symbol->isVar(), hVar);
    hType = max(symbol->isType(), hType);
    size++;
    return true;
}

Symbol *SymTable::find(const string &name)
{
    for (Symbol* s : table) {
        if (s->getName() == name) {
            return s;
        }
    }
    return NULL;
};

Symbol *SymTable::top()
{
    return table.size() > 0 ? table.back() : NULL;
}

void SymTable::printTypes(int deep, bool printType)
{
    for (Symbol* s : table) {
        if (s->isType()) {
            s->print(deep);
            cout << endl;
        }
    }
}

void SymTable::printVariables(int deep, bool printType)
{
    for (Symbol* s : table) {
        if (s->isVar()) {
            if (!dynamic_cast<SymVar *>(s)->isParam()) {
                s->print(deep);
            }
        }
    }
}

void SymTable::printFunctions(int deep, bool printType)
{
    for (Symbol* s : table) {
        if (s->isFunc()) {
            s->print(deep);
        }
    }
}

void SymTable::printParam(int deep, bool printType)
{
    for (Symbol* s : table) {
        if (s->isVar()) {
            if (dynamic_cast<SymVar *>(s)->isParam()) {
                s->print(deep);
            }
        }
    }
}

bool SymTable::hasAnonymousSym()
{
    for (Symbol* s : table) {
        if(s->isAnonymousSym()){
            return true;
        }
    }
    return false;
}

void SymTable::print(int deep, bool printType)
{
    string tab = string(deep, ' ');
    if (hasType()) {
        cout << tab << "+----Types--+" << endl;
        printTypes(deep + N, printType);
    }
    if (hasVar()) {
        cout << tab << "+----Variables----+" << endl;
        printVariables(deep + N, printType);
    }
    if (hasFunc()) {
        cout << tab << "+----Functions----+" << endl;
        printFunctions(deep + N,printType);
    }
}

//--------------------------------symStack--------------------------------//


void SymTableStack::push(SymTable* table)
{
    tables.push_back(table);
    
};

void SymTableStack::pop()
{
    tables.pop_back();
}

SymTable* SymTableStack::top()
{
    return (tables.size() > 0) ? tables.back() : NULL;
}

void SymTableStack::add(Symbol* s)
{
    if (s == NULL) {
        return;
    }
    if (!top()->add(s)) {
        throw parser_exception("Redefinition " + s->getName(), false);
    }
}

Symbol* SymTableStack::find(const string &name)
{
    for (int i = (int)tables.size() - 1; i >= 0; i--) {
        Symbol* s = tables[i]->find('$' + name);
        if (s != NULL) {
            return s;
        };
        s = tables[i]->find(name);
        if (s != NULL) {
            return s;
        };
    }
    return NULL;
};

void SymTableStack::print(int deep, bool printType)
{
    string tab = string(deep, ' ');
    if (top()->hasType()) {
        cout << tab << "+----Types----+" << endl;
        printTypes(deep + N);
    }
    if (top()->hasVar()) {
        cout << tab << "+----Variables----+" << endl;
        printVariables(deep + N);
    }
    if (top()->hasFunc()) {
        cout << tab << "+----Functions----+" << endl;
        printFunctions(deep + N);
    }
}

void SymTableStack::printTypes(int deep, bool printType)
{
    for (SymTable* st : tables){
        st->printTypes(deep, printType);
    }
};

void SymTableStack::printVariables(int deep, bool printType)
{
    for (SymTable* st : tables){
        st->printVariables(deep, printType);
    }
};

void SymTableStack::printFunctions(int deep, bool printType)
{
    for (SymTable* st : tables){
        st->printFunctions(deep, printType);
    }
};

//---------------------------------symbols---------------------------------//


void SymType::print(int deep, bool printType)
{
    cout << string(deep, ' ') << getName();
}


void SymVar::print(int deep, bool printType)
{
    cout << string(deep, ' ');
    if (isConst()) {
        cout << " const ";
    }
    cout << " " << getName();
    cout << " ";
    type->print(0, !type->isStruct());
    if (exp != NULL) {
        cout << " = ";
        exp->print(deep, false);
    }
    cout << endl;
}

bool SymTypeScalar::canConvertTo(SymType *newType)
{
    if (dynamic_cast<SymTypePointer*>(newType) || dynamic_cast<SymFunc*>(newType))
        return false;
    return this->getPriority() <= newType->getPriority();
}

void SymTypeArray::print(int deep, bool printType)
{
    cout << "array [" ;
    if(size != NULL)
        size->print(0,false);
    cout << "] of ";
    if (type->isFunc()) {
        dynamic_cast<SymFunc *>(type)->printAsType(deep, printType);
    } else if (type != NULL) {
        type->print(deep, !type->isStruct() && printType);
    }
}

bool SymTypeArray::canConvertTo(SymType *newType)
{
    if (  dynamic_cast<SymTypeArray*>(newType) || dynamic_cast<SymTypeInt *>(newType))
        return true;
    SymTypePointer* p = dynamic_cast<SymTypePointer*>(newType);
    if (p && *p->getNextType() == type)
        return true;
    return false;
}

bool SymTypeArray::operator==(SymType* t)
{
    SymTypeArray* a = dynamic_cast<SymTypeArray*>(t);
    if (!a)
        return false;
    return size == a->getSize() && *type == a->type;
}


void SymTypePointer::print(int deep, bool printType)
{
    cout << (isConst() ? "const " : "" ) <<"pointer to ";
    if (type->isFunc()) {
        dynamic_cast<SymFunc *>(type)->printAsType(deep, printType);
    } else {
        type->print(deep, !type->isStruct() && printType);
    }
}

bool SymTypePointer::canConvertTo(SymType *newType)
{
   	if (newType == intType || newType->isArray())
        return true;
    SymType* p = dynamic_cast<SymTypePointer*>(newType);
    if (p)
        return *this == p;
    return false;
}

bool SymTypePointer::operator==(SymType* t)
{
    SymTypePointer* p = dynamic_cast<SymTypePointer*>(t);
    if (!p)
        return false;
    SymType* t1 = getNextType();
    SymType* t2 = p->getNextType();
    while (true)
    {
        if (!(*t1 == t2))
            return false;
        SymType* tmp1 = t1->getNextType();
        SymType* tmp2 = t1->getNextType();
        if (tmp1 == NULL || tmp2 == NULL)
        {
            if (tmp1 != NULL || tmp2 != NULL)
                return false;
            return true;
        } else {
            t1 = tmp1;
            t2 = tmp2;
        }
    }
    return false;
}

void SymTypeStruct::print(int deep, bool printType)
{
    cout << string(deep, ' ') <<"struct " << getName();
    if (table != NULL && printType) {
        cout << (table->getSize() > 0 ? " :\n" : "");
        table->print(deep + N + N);
    }
    //cout << string(deep + N, ' ') << "+-----------------+";
}

Symbol *SymTypeStruct::find(const string &name)
{
    return table->find(name);
}

void SymFunc::print(int deep, bool printType)
{
    cout <<  string(deep, ' ');
    if (retType->isStruct()) {
        cout << retType->getName();
    } else if(retType->isFunc()){
        dynamic_cast<SymFunc*>(retType)->printAsType();
    } else
        retType->print();
    cout << " " << getName()<< "(";
    if (args->getSize() > 0 && printType) {
        cout << endl;
        args->printParam(deep + N);
        cout << string(deep, ' ');
    }
    cout << ")";
    if (body != NULL && printType) {
        cout << endl << string(deep, ' ') <<"{" << endl;
        body->print(deep);
        cout << endl << string(deep, ' ') <<"}" << endl;
    } else {
        cout << endl;
    }
    
}

void SymFunc::setType(SymType *type)
{
//    if (type != NULL && type->getType() != NULL && type->getType()->isArray()) {
//        throw parser_exception("Function cannot return array type ");
//    }
    retType = type;
}

void SymFunc::printAsType(int deep, bool printType)
{
    cout << "function (";
    if (args->getSize() > 0) {
        cout << endl;
        args->printParam(deep + N);
        cout << string(deep, ' ');
    }
    cout << ")";

    cout << " returning ";
    retType->print(deep, printType);
}

void SymTypeDef::print(int deep, bool printType)
{
    cout << string(deep, ' ') << getName() << " ";
    type->print();
}
