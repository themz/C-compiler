#include <stdio.h>


// SymType *Parser::parseFunctionDeclaration(SymType *type, bool parseParam)
// {
//     NL();
//     StmtBlock *body = NULL;
//     SymTable *params = parseFunctionsParams();
//     if(*GL() == BRACE_FRONT){
//         exception("Expected ')' in function declaration can not have body", parseParam);
//         exception("Parameters name omitted", params->hasAnonymousSym());
//         body = parseBlock(params);
//     } else if(*GL() == SEMICOLON || *GL() == COMMA || *GL() == BRACKET_FRONT || *GL() == PARENTHESIS_BACK || *GL() == PARENTHESIS_FRONT){
        
//     }
//     else {
//         exception("Expected function body, complexDeclaration or ';' but not '" + GL()->getValue() + "'");
//     }
//     string s = "";
//     return new SymFunc(s, type, params, body);
// }

// void Parser::parseSemicolon()
// {
//     while (*GL() == SEMICOLON) {
//         NL();
//     }
// }

// SymTable* Parser::parseFunctionsParams()
// {
//     SymTable *table = new SymTable();
//     SymType *type = NULL;
//     while (*GL() != PARENTHESIS_BACK) {
//         bool isConst = false;
        
//         if (*GL() == T_CONST) {
//             NL();
//             isConst = true;
//         }
//         state->push(PARSE_FUNC_ARG_DEF);
//             type = hitch(parseType(), parseDeclarator()) ;
//         state->pop();
        
//         string name = names->pop();
//         exception("C does not support default arguments", *GL() == ASSIGN);
//         exception("Redefinition param name:  \"" + name + "\"", !table->add(new SymVar(name, type, NULL, isConst, true)));
//         exception("Expected ',' or identificator", *GL() != COMMA && *GL() != PARENTHESIS_BACK);
//         if (*GL() == COMMA) {
//             NL();
//         }
//     }
//     NL();
//     return table;
// }

//--------------------------------------------------------------------------------------------


// int foo();
// int a, **bar(), baz(int **, float y), b;

// int foo() { return 0;};
// int baz(int **x, float y) {return 0;}

// struct strct {int i;};
// struct strct **foobar(struct strct s, struct strct **x[10]) {
//  	struct strct **a;
//  	return a;
//  }
//  struct strct **foobar(struct strct, struct strct **x[10]);

//  typedef struct strct ***tstrct[3];
 //tstrct qux(tstrct);
 // tstrct qux(struct strct ***x[3]);

/*
int a1;
int *a2;
int (*a)[];
int ***a3[1][1][1];
int *a4[2];
int *a5[2][2];
int **a6[2][6];

//int *(*a7[1])[1];
//int *(*a7[1])[1];
//int **(*(*x)[3])[5][5];
//int a()[];


int main()
{
	char (*(*x[3])())[5];
	char * const (*(* const const (*(* const bar)[5])(int ))[5])(int );

}
*/
// typedef int (*(*foo2(float (a), float b)))(int a, int b);
int main()
{
	void f3(int** (*(*x())[][3])());
	int** (*(*z())[][3])[];
	f3(z);
	return 0;
}


