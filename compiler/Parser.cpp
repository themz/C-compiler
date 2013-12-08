#include "Parser.h"

Parser::Parser(Scanner &scanner):scanner_(scanner)
{
	needNext = true;
	priorityTable[PARENTHESIS_FRONT] = 15;
	priorityTable[BRACKET_FRONT] = 15;
	priorityTable[ARROW] = 15;
	priorityTable[DOT] = 15;
	priorityTable[LOGICAL_NOT] = 14;
	priorityTable[BITWISE_NOT] = 14;
	priorityTable[INC] = 14;
	priorityTable[DEC] = 14;
	priorityTable[MULT] = 13;
	priorityTable[DIV] = 13;
	priorityTable[MOD] = 13;
	priorityTable[PLUS] = 12;
	priorityTable[MINUS] = 12;
	priorityTable[BITWISE_SHIFT_LEFT] = 11;
	priorityTable[BITWISE_SHIFT_RIGHT] = 11;
	priorityTable[LESS] = 10;
	priorityTable[LESS_OR_EQUAL] = 10;
	priorityTable[GREATER] = 10;
	priorityTable[GREATER_OR_EQUAL] = 10;
	priorityTable[EQUAL] = 9;
	priorityTable[NOT_EQUAL] = 9;
	priorityTable[BITWISE_AND] = 8;	
	priorityTable[BITWISE_XOR] = 7;
	priorityTable[BITWISE_OR] = 6;
	priorityTable[LOGICAL_AND] = 5;
	priorityTable[LOGICAL_OR] = 4;
	priorityTable[QUESTION] = 3;
	priorityTable[ASSIGN] = 2;
	priorityTable[PLUS_ASSIGN] = 2;
	priorityTable[MINUS_ASSIGN] = 2;
	priorityTable[MULT_ASSIGN] = 2;
	priorityTable[DIV_ASSIGN] = 2;
	priorityTable[MOD_ASSIGN] = 2;
	priorityTable[AND_ASSIGN] = 2;
	priorityTable[OR_ASSIGN] = 2;
	priorityTable[BITWISE_XOR_ASSIGN] = 2;
	priorityTable[AND_ASSIGN] = 2;
	priorityTable[OR_ASSIGN] = 2;
	priorityTable[BITWISE_SHIFT_LEFT_ASSIGN] = 2;
	priorityTable[BITWISE_SHIFT_RIGHT_ASSIGN] = 2;
	priorityTable[COMMA] = 1;
	priorityTable[COLON] = 0; 

	unaryOps[LOGICAL_NOT] = true;
	unaryOps[MULT] = true; 
	unaryOps[INC] = true;
	unaryOps[DEC] = true;
	unaryOps[BITWISE_AND] = true; 
	unaryOps[PLUS] = true;
	unaryOps[MINUS] = true;
	unaryOps[BITWISE_NOT] = true;

	rightAssocOps[ASSIGN] = true;
	rightAssocOps[PLUS_ASSIGN] = true;
	rightAssocOps[MINUS_ASSIGN] = true;
	rightAssocOps[MULT_ASSIGN] = true;
	rightAssocOps[DIV_ASSIGN] = true;
	rightAssocOps[MOD_ASSIGN] = true;
	rightAssocOps[AND_ASSIGN] = true;
	rightAssocOps[OR_ASSIGN] = true;
	rightAssocOps[BITWISE_XOR_ASSIGN] = true;
	rightAssocOps[BITWISE_SHIFT_LEFT_ASSIGN] = true;
	rightAssocOps[BITWISE_SHIFT_RIGHT_ASSIGN] = true;
	rightAssocOps[LOGICAL_NOT] = true;
	rightAssocOps[BITWISE_NOT] = true;
	rightAssocOps[MULT] = true;
	rightAssocOps[BITWISE_AND] = true;
	rightAssocOps[QUESTION] = true;
}

Node* Parser::parseExp(int priority){
        needNext = true;	
		if (priority > 15)
			return parseFactor();
		Node* l = parseExp(priority + 1);
        Node* root = l;
        Lexeme* lex = needNext ? scanner_.getNextLex() : scanner_.get();
		needNext = false;	
		if (!lex || *lex == ENDOF || *lex == PARENTHESIS_BACK || *lex == BRACKET_BACK || *lex == COLON || *lex == SEPARATOR){
			return root;
        }
        OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
		while(opLex && priorityTable[opLex->getOpType()] >= priority)
		{
			OperationType op = opLex->getOpType();
			switch (op)
			{
			case(INC):
			case(DEC):
				needNext = true;
				root = new PostfixUnaryOpNode(opLex, root);
				break;
			case(QUESTION):
				{
					Node* l = parseExp();
					Lexeme* le = scanner_.get(); 
					OperationLexeme* sepLex = dynamic_cast<OperationLexeme*>(le);
					if (*sepLex != COLON)
						throw parser_exception("Missed branch of ternary operator", scanner_.getLine(), scanner_.getCol());
					Node* r = parseExp();
					root = new TernaryOpNode(sepLex, root, l, r);
				}
				break;
			//case(DOT):
			//case(ARROW):
			default:
				needNext = true;
				root = new BinaryOpNode(opLex, root, parseExp(priority + (rightAssocOps[op] ? 0 : 1)));		
				break;
			}
		opLex = dynamic_cast<OperationLexeme*>(needNext ? scanner_.getNextLex() : scanner_.get());
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
	case(CHAR):
		root = new CharNode(lex);
	case RESERVEDWORD:
		{
			ReservedWordType rwType = dynamic_cast<ReservedWordLexeme*>(lex)->getRwType();
			if (rwType == T_CHAR || rwType == T_INT || rwType == T_DOUBLE)
			{
				OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(scanner_.getNextLex()); 
				if (!opLex || opLex->getOpType() != PARENTHESIS_FRONT)
					throw parser_exception("Expected open parenthesis", scanner_.getLine(), scanner_.getCol());

				root = new TypecastNode(lex, parseExp());

				opLex = dynamic_cast<OperationLexeme*>(scanner_.getNextLex(false)); 
				if (!opLex || opLex->getOpType() != PARENTHESIS_BACK)
					throw parser_exception("Expected close parenthesis", scanner_.getLine(), scanner_.getCol());
				needNext = true;
			} 
			else if (rwType == T_SIZEOF)
			{
			
			}
			else 
				throw parser_exception("Expected typecast word: int(), double(), char() or sizeof()", scanner_.getLine(), scanner_.getCol());
			break;
		}
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
		else if (unaryOps[opLex->getOpType()]) 
		{
				needNext = true;
				root = new UnaryOpNode(lex, parseExp(priorityTable[DEC]));
		}
		else 
		{
			throw parser_exception ("Empty expression is not allowed", false);
		}
	}
	return root;
}
