#include "Symbols.h"
#include <iostream>

#define N 3

void StmtBlock::addStmt(Stmt *statement)
{
    statements.push_back(statement);
}

void StmtBlock::print(int deep)
{
    table->print(deep);
    for (Stmt* st : statements){
        cout << endl << string(deep + N, ' ') <<"{" << endl;
        st->print(deep + N);
        cout << endl << string(deep + N, ' ') <<"}" << endl;

    }
}
