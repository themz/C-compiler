#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
    string readme = "c-compiler {/s | /ex | /p | /go} file.in \n"
                    "\t/s\tLexical analysis & print lexems\n"
                    "\t/ex\tExpression parser & print expression tree \n"
                    "\t/p\tParse file and print symbols table\n"
                    "\tfile.in\tSource filename witn .in\n";
    string copyRight = "C-compiler v.0.8 developed by Zinov Mikhail 2014";
    bool debug = true;
    if (argc == 4) {
        debug = string(argv[3]) != string("ok");
    }
	try{
		if (!debug)
		{		
			if (argc > 1)
			{
                string filename = argv[2];
                freopen((filename.substr(0, filename.length() - 3) +(string) ".out").c_str() , "w", stdout);
                Scanner scanner(filename);
                if (string(argv[1]) == string("/s"))
				{
                    while (scanner.next())
                    {
                        scanner.get()->print();
                    }				
				}
				else if (string(argv[1]) == string("/px"))
				{
                    Parser p(scanner);
                    p.cState = PARSE_EXP;
                    Node* r = p.parseExp();
					if (r)
					{
						r->print();
					}
				}
                else if (string(argv[1]) == string("/p"))
                {
                    Parser parser(scanner);
                    parser.cState = PARSE_EXP_SYM;
                    parser.parse();
                    parser.printTable();
                }
				else
				{
					cout << "Expected /s or /px or /p" << endl;
				}
			}
			else
			{
	 			cout << readme << endl << copyRight << endl;
			}
		}
		else
		{
            string filename = "0.in";
            Scanner scanner(filename);
            //freopen("0.out", "w", stdout);
            switch (3) {
                case 1:
                {
                    Parser p(scanner);
                    p.cState = PARSE_EXP;
                    Node* r =  p.parseExp();
                    if (r)
                    {
                        r->print();
                    }
                    break;
                }
                case 2:
                {
                    while (scanner.next())
                    {
                        scanner.get()->print();
                    }
                    break;
                }
                case 3:
                {
                    Parser parser(scanner);
                    parser.cState = PARSE_EXP_SYM;
                    parser.parse();
                    parser.printTable();
                    break;
                }
                default:
                    break;
            }
		}
	}
	catch(compiler_exception& exc)
	{
		cout << "Error: " << exc.getExceptionMsg() << endl;
		if (exc.getPrintingPos())
		{
			cout << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
		}
	}
	return 0;
}

