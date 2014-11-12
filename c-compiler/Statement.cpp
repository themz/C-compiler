#include "Symbols.h"
#include <iostream>

#define N 3

void StmtBlock::addStmt(Stmt *statement)
{
    statements.push_back(statement);
}

void StmtBlock::print(int deep)
{
    table->print(deep + N);
    for (Stmt* st : statements){
        st->print(deep);
    }
}
