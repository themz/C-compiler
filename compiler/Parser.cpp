#include "Parser.h"

Parser::Parser(Scanner &scanner):scanner_(scanner)
{
	needNext = true;	
}
void Parser::parse()
{
	
}

Node* Parser::parseExp(int priority){
	needNext = true;
	Node* l = parseTerm();
	Node* root = l;
	Lexeme* lex = needNext ? scanner_.getNextLex() : scanner_.get();
	if (*lex == ENDOF)
	{
		return root;
	}
	OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
	if ( opLex != NULL && (*opLex == PLUS || *opLex == MINUS))
	{
			return new BinaryOpNode(lex, l, parseExp());
	}

		//else
		//	throw parser_exception ("Unsup lex!");
	//else
	//	throw parser_exception ("Invalid scanner!");
	return root;
} 

Node* Parser::parseTerm(int priority)
{
	Node* l = parseFactor();
	Node* root = l; 
	Lexeme* lex = needNext ? scanner_.getNextLex() : scanner_.get();
	if (*lex == ENDOF)
	{
		return root;
	}
	OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
	if (opLex &&(*opLex == DIV ||  *opLex == MOD || *opLex == MULT))
	{
			return new BinaryOpNode(lex, l, parseTerm());	
	}
	else
	{
		//needNext = false;
		//	throw parser_exception ("This Lexem is unsupported!");
	}
	//else
	//	throw parser_exception ("Invalid scanner!");
	return root;
}


Node* Parser::parseFactor(int priority)
{
	Node* root = NULL;
	Lexeme* lex = scanner_.getNextLex();
	if (*lex == ENDOF)
	{
		return root;
	}
	switch (lex->getLexType())
	{
	case(IDENTIFICATOR):
		root = new IdentifierNode(lex);
		break;
	case(INTEGER):
		root = new IntNode(lex);
		break;
	case(DOUBLE):
		root = new DoubleNode(lex);
	case(OPERATION):
		OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
		if (*opLex == PARENTHESIS_FRONT)
		{
			root = parseExp();
			Lexeme* lex = scanner_.getNextLex();
			OperationLexeme* clLex = dynamic_cast<OperationLexeme*>(lex);
			if(!clLex || *clLex != PARENTHESIS_BACK)
			{
				throw parser_exception ("Expected parenthesis close");
			}
		}
		else 

		{
			throw parser_exception ("Empty expression is not allowed");
		}
	}
	return root;
}