#include "Parser.h"

Parser::Parser(Scanner &scanner): scanner_(scanner)
{
	
}

Node* Parser::parseExp(int priority){
	Lexeme* lex = NULL;
	Node* l = parsTerm();

	if (scanner_.next())
	{
		lex = scanner_.get();
		if (dynamic_cast<OperationLexeme*>(lex)->getTVal() == PLUS || dynamic_cast<OperationLexeme*>(lex)->getTVal() == MINUS)
		{
			cout << 123;
			system("pause");
		}  

	}
	else
		throw parser_exception ("Invalid scanner!");
	return l;
} 

Node* Parser::parsTerm(int priority)
{
	Node* a = new Node();
	return a;
}

Node* Parser::parsFactor(int priority)
{
	Node* a = new Node();
	return a;
}