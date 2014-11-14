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
    //cout << string(deep + N, ' ') << "+----Stmt----+" << endl;
    if (statements.size() > 0) {
        cout << endl << string(deep + N, ' ') <<"{" << endl;
        for (Stmt* st : statements){
            st->print(deep + N);
        }
        cout << string(deep + N, ' ') <<"}" << endl;
    }
}

void StmtIf::print(int deep)
{
    cout << string(deep + N, ' ') << "if (";
    condition->print(0, false);
    cout <<  ") {" << endl;
    trueBlock->print(deep + N);
    cout << string(deep + N, ' ') <<  "}" ;
    if (falseBlock != NULL) {
        cout << " else {" << endl;
        falseBlock->print(deep + N);
        cout << string(deep + N, ' ') <<  "}";
    }
    cout << endl;
}

void StmtSingle::print(int deep)
{
    cout << string(deep + N, ' ') ;
    if (exp != NULL) {
        exp->print(0, false);
    } else {
        cout << "Empty single stmt";
    }
    cout << endl;

}

void StmtFor::print(int deep)
{
    cout << string(deep + N, ' ') << "for (";
    if (init != NULL) {
        init->print(0,false);
    }
    cout << ";";
    if (condition != NULL) {
        condition->print(0,false);
    }
    cout << ";";

    if (inc != NULL) {
        inc->print(0,false);
    }
    cout << ")";
    if (body != NULL) {
        cout << "{" << endl;
        body->print(deep + N);
        cout << endl << string(deep + N, ' ') <<  "}" << endl;
    } else {
        cout << ";";
    }
}

void StmtWhile::print(int deep)
{
    cout << string(deep + N, ' ') << "while (";
    condition->print(0,false);
    cout << ")";
    if (body != NULL) {
        cout << "{" << endl;
        body->print(deep + N);
        cout << endl << string(deep + N, ' ') <<  "}" << endl;
    } else {
        cout << ";";
    }
}

void StmtDoWhile::print(int deep)
{
    cout << string(deep + N, ' ') << " do ";
    if (body != NULL) {
        cout << "{" << endl;
        body->print(deep + N);
        cout << endl << string(deep + N, ' ') <<  "}";
    } else {
        cout << ";";
    }
    cout << " while( ";
    condition->print(0,false);
    cout << ")" << endl;
}


void StmtReturn::print(int deep)
{
    cout << string(deep + N, ' ') << "return ";
    if(retArg != NULL)
    {
        retArg->print(0, false);
    } else {
        cout << ";";
    }
    cout << endl;
}

void StmtBreak::print(int deep)
{
    cout << string(deep + N, ' ') << "break;" << endl;
}

void StmtContinue::print(int deep)
{
    cout << string(deep + N, ' ') << "continue;" << endl;
}





