#include "Scanner.h"
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[])
{
	bool debug = false;
	Scanner scanner;
	FILE *inp,*outp;
	if (!debug)
	{		
		if (argc > 1)
		{
			if ((string)argv[1] == (string)"/S")
			{
				string fname = string(argv[2]);
				freopen_s(&inp, fname.c_str(), "r", stdin);
				freopen_s(&outp, (fname.substr(0, fname.length() - 3) +(string) ".out").c_str() , "w", stdout);
				try{
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
				cout << argv[1] << 1;			
			}
		}
		else
		{
	 		cout << "C-compilator pf-pf-pf" << endl;
		}
	}
	else
	{
		freopen_s(&inp, "1.in" , "r", stdin);
		freopen_s(&outp, "1.sol" , "w", stdout);
		try{
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
	return 0;
}

