#include "Symbols.h"

#define N 2
//--------------------------------SymTable--------------------------------//

bool SymTable::add(Symbol *symbol)
{
    if (find(symbol->getName()) != NULL && symbol->getName() != "") {
        return false;
    }
    table.push_back(symbol);
    hFunc = max(symbol->isFunc(), hFunc);
    hVar = max(symbol->isVar(), hVar);
    hType = max(symbol->isType(), hType);
    size++;
    return true;
}

Symbol* SymTable::find(const string &name)
{
    for (Symbol* s : table) {
        if (s->getName() == name) {
            return s;
        };

    }
    return NULL;
};

void SymTable::printTypes(int deep)
{
    for (Symbol* s : table) {
        if (s->isType()) {
            s->print(deep);
        }
    }
}

void SymTable::printVariables(int deep)
{
    for (Symbol* s : table) {
        if (s->isVar()) {
            s->print(deep);
        }
    }
}

void SymTable::printFunctions(int deep)
{
    for (Symbol* s : table) {
        if (s->isFunc()) {
            s->print(deep);
        }
    }
}

void SymTable::print(int deep)
{
    string tab = string(deep, '-');
    if (hasType()) {
        cout << std::endl << tab << "TYPES:\n";
        printTypes(deep + N);
    }
    if (hasVar()) {
        cout << std::endl << tab << "VARIABLES:\n";
        printVariables(deep + N);
    }
    if (hasFunc()) {
        cout << std::endl << tab << "FUNCTIONS:";
        printFunctions(deep + N);
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
    top()->add(s);
}

Symbol* SymTableStack::find(const string &name)
{
    for (SymTable* st : tables) {
        Symbol* s = st->find(name);
        if (st != NULL) {
            return s;
        };
    }
    return NULL;
};

void SymTableStack::print(int deep)
{
    string tab = string(deep, '-');
    if (top()->hasType()) {
        cout << tab << "TYPES:\n";
        printTypes(deep + N);
    }
    if (top()->hasVar()) {
        cout << std::endl << tab << "VARIABLES:\n";
        printVariables(deep + N);
    }
    if (top()->hasFunc()) {
        cout << std::endl << tab << "FUNCTIONS:";
        printFunctions(deep + N);
    }
}

void SymTableStack::printTypes(int deep)
{
    for (SymTable* st : tables){
        st->printTypes(deep);
    }
};

void SymTableStack::printVariables(int deep)
{
    for (SymTable* st : tables){
        st->printVariables(deep);
    }

};

void SymTableStack::printFunctions(int deep)
{
    for (SymTable* st : tables){
        st->printFunctions(deep);
    }
};

//---------------------------------symbols---------------------------------//


void SymType::print(int deep)
{
    std::cout << string(deep, ' ') << "type with name " << getName() << std::endl;
}


void SymVar::print(int deep)
{
    std::cout << string(deep, ' ');
    if (isConst()) {
        std::cout << "const ";
    }
    std::cout << "variable \"" << getName() <<"\" with type \"" << type->getName() << "\"";
    if (exp != NULL) {
        std::cout << " and values: "<< std::endl;
        exp->print();
    } else {
        std::cout << " not initialized" << std::endl;
    }
    
}

void SymTypeStruct::print(int deep)
{
    cout << string(deep, ' ') <<"struct with name \"" << getName() << "\" with table:";
    table->print(deep);
}

void SymFunc::print(int deep)
{
    std::cout << "\nfunction return type \"" << retType->getName() << "\" with name "
    << getName();
    if (args->getSize() > 0) {
        std::cout << " and args: "<< std::endl;
    args->printVariables(deep + N);
    //std::cout << endl;
    };
}




































































































