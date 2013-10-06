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

	 
int main(int argc, char* argv[])
{
	Scanner scanner;
	FILE *inp,*outp;
    freopen_s(&inp, "0.in", "r", stdin);
    freopen_s(&outp, "0.out", "w", stdout);
	while (scanner.next())
	{
		scanner.get().print();
	}
	return 0;
}

