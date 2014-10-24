#include "Symbols.h"


//--------------------------------SymTable--------------------------------//

bool SymTable::add(Symbol *symbol)
{
    if (find(symbol->getName()) != NULL) {
        return false;
    }
    table.push_back(symbol);
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

void SymTable::printTypes()
{
    for (Symbol* s : table) {
        if (s->isType()) {
            s->print();
        }
    }
}

void SymTable::printVariables()
{
    for (Symbol* s : table) {
        if (s->isVar()) {
            s->print();
        }
    }
}

void SymTable::printFunctions()
{
    for (Symbol* s : table) {
        if (s->isFunc()) {
            s->print();
        }
    }
}

void SymTable::print()
{
    cout << std::endl << "=================================== TYPES ===================================\n\n";
    printTypes();
    cout << std::endl << "================================= VARIABLES =================================\n\n";
    printVariables();
    cout << std::endl << "================================= FUNCTIONS =================================\n\n";
    printFunctions();
    
}

//--------------------------------symStack--------------------------------//


void SymTableStack::push(SymTable* table)
{
    tables.push_back(table);
    
};

SymTable* SymTableStack::top()
{
    return (tables.size() > 0) ? tables.back() : NULL;
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

void SymTableStack::print()
{
    cout << std::endl << "=================================== TYPES ===================================\n\n";
    printTypes();
    cout << std::endl << "================================= VARIABLES =================================\n\n";
    printVariables();
    cout << std::endl << "================================= FUNCTIONS =================================";
    printFunctions();
   
}

void SymTableStack::printTypes()
{
    for (SymTable* st : tables){
        st->printTypes();
    }
};

void SymTableStack::printVariables()
{
    for (SymTable* st : tables){
        st->printVariables();
    }

};

void SymTableStack::printFunctions()
{
    for (SymTable* st : tables){
        st->printFunctions();
    }
};

//---------------------------------symbols---------------------------------//


void SymType::print()
{
    std::cout << "Type with name " << getName() << std::endl;
}


void SymVar::print()
{
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

void SymFunc::print()
{
    std::cout << "function with return type " << retType->getName() << "with name "
    << getName() << " args: "<< std::endl;
    args->print();
    std::cout << endl;
}




































































































