#include "Parser.h"

Parser::Parser(Scanner &scanner):scanner_(scanner)
{
	needNext = true;	
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
		needNext = false;
	}
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
		break;
	case(OPERATION):
		OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
		if (*opLex == PARENTHESIS_FRONT)
		{
			root = parseExp();
			needNext = true;
			Lexeme* lex = scanner_.get();
			OperationLexeme* clLex = dynamic_cast<OperationLexeme*>(lex);
			if(!clLex || *clLex != PARENTHESIS_BACK)
			{
				throw parser_exception ("Expected parenthesis close", false);
			}
		}
		else 
		{
			throw parser_exception ("Empty expression is not allowed", false);
		}
	}
	return root;
}