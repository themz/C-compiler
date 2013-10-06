#include "Scanner.h"
#include <stdio.h>

using namespace std;

string typeDef[] = {
	"ReservedWord",
	"Identificator",
	"Integer",
	"Float",
	"Error",
	"Separator",
	"Operation",
	"Char",
	"String"
};

	 
int main()
{
	Scanner textScanner;
	FILE *inp,*outp;
    freopen_s(&inp,"0.in","r",stdin);
    freopen_s(&outp,"0.out","w",stdout);
	while (textScanner.next())
	{
		Lexeme lex = textScanner.get();
		lex.print();
	}
	return 0;
}

