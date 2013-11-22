#include "Scanner.h"
#include "Parser.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	bool debug = 0;
	FILE *outp;
	if (!debug)
	{		
		if (argc > 1)
		{
			if (argv[1] == "/S")
			{
				Scanner scanner(argv[2]);
				try{
					freopen_s(&outp, (scanner.getFileName().substr(0, scanner.getFileName().length() - 3) +(string) ".out").c_str() , "w", stdout);
					while (scanner.next())
					{
						scanner.get()->print();
					}
				}
				catch(compiler_exception& exc)
				{
					cout << "Exeption: " << exc.getExceptionMsg() << endl << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
				}
			}
			else if (argv[1] == "/PS")
			{
				Parser pars(Scanner(argv[2]));				
			}
			else
			{
				cout << "C-compilator v.0.3 developed by Zinov Mikhail 2013" << endl;				
			}
		}
		else
		{
	 		cout << "C-compilator v.0.3 developed by Zinov Mikhail 2013" << endl;
		}
	}
	else
	{
		try{
			Scanner scanner("0.in");
			freopen_s(&outp, "0.out", "w", stdout);					
			while (scanner.next())
			{
				scanner.get()->print();
			}
		}
		catch(compiler_exception& exc)
		{
			cout << "Exeption: " << exc.getExceptionMsg() << endl << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
		}

		system("pause");
	}	
	return 0;
}

