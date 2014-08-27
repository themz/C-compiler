#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	string copyRight = "C-compilator v.0.4 developed by Zinov Mikhail 2014";
	
	bool debug = 0;	
	FILE *outp;
	try{
		if (!debug)
		{		
			if (argc > 1)
			{
				if (string(argv[1]) == string("/S"))
				{
					Scanner scanner(argv[2]);				
						freopen_s(&outp, (scanner.getFileName().substr(0, scanner.getFileName().length() - 3)+(string)".out").c_str() , "w", stdout);
						while (scanner.next())
						{
							scanner.get()->print();
						}				
				}
				else if (string(argv[1]) == string("/PS"))
				{
					string filename = argv[2];
					freopen_s(&outp, (filename.substr(0, filename.length() - 3) +(string) ".out").c_str() , "w", stdout);
					Parser p = Parser(Scanner(filename));					
					Node* r = p.parseExp();
					if (r)
					{
						r->print();
					}
				}
				else
				{
					cout << copyRight << endl;				
				}
			}
			else
			{
	 			cout << copyRight << endl;
			}
		}
		else
		{
		bool tpars = true;
			if (tpars)
			{					
				string filename = "0.in";
				Node* r =  Parser(Scanner(filename)).parseExp();					
				if (r)
				{
					r->print();
				}
			}
			else
			{			
				Scanner scanner("0.in");
				freopen_s(&outp, "0.out", "w", stdout);					
				while (scanner.next())
				{
					scanner.get()->print();
				}			
			}		
		}
	}
	catch(compiler_exception& exc)
	{
		cout << "Exeption: " << exc.getExceptionMsg() << endl;
		if (exc.getPrintingPos())
		{
			cout << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
		}
	}
	if (debug) system("pause");	
	return 0;
}

