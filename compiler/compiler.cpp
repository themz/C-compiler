#include "Scanner.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	bool debug = false;
	FILE *outp;
	if (!debug)
	{		
		if (argc > 1)
		{
			if ((string)argv[1] == (string)"/S")
			{
				Scanner scanner(argv[2]);
				try{
					freopen_s(&outp, (scanner.getFileName().substr(0, scanner.getFileName().length() - 3) +(string) ".out").c_str() , "w", stdout);
					while (scanner.next())
					{
						scanner.get()->print();
					}
					if(!scanner.get()){
						throw compiler_exception ("Empty file ", 0, 0); 
					}
				}
				catch(compiler_exception& exc)
				{
					cout << "Exeption: " << exc.getExceptionMsg() << endl << "row:" << exc.getExRow()<< " " << "col:" << exc.getExCol();
				}
			}
			else
			{
				cout << "C-compilator v.0.2 developed by Zinov Mikhail 2013" << endl;				
			}
		}
		else
		{
	 		cout << "C-compilator v.0.2 developed by Zinov Mikhail 2013" << endl;
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
			if(!scanner.get()){
				throw compiler_exception ("Empty file ", 0, 0); 
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

