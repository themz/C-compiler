#include "Parser.h"

Parser::Parser(Scanner &scanner):scanner_(scanner)
{
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
    
    scanner_.nextLex();
}

Node* Parser::parseExp(int priority){
    if (priority > 15)
            return parseFactor();
    Node* l = parseExp(priority + 1);
    Node* root = l;
    Lexeme* lex = scanner_.get();
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
                root = new PostfixUnaryOpNode(opLex, root);
                scanner_.nextLex();
                break;
            case(PARENTHESIS_FRONT):
                root = parseFuncCall(root);
                break;
            case(BRACKET_FRONT):
                root = parseArrIndex(root);
                break;
            case(QUESTION):
            {
                scanner_.nextLex();
                Node* l = parseExp();
                Lexeme* le = scanner_.get(); 
                OperationLexeme* sepLex = dynamic_cast<OperationLexeme*>(le);
                if (*sepLex != COLON)
                    throw parser_exception("Missed branch of ternary operator", scanner_.getLine(), scanner_.getCol());
                scanner_.nextLex();
                Node* r = parseExp();
                root = new TernaryOpNode(sepLex, root, l, r);
                break;
            }
        //case(DOT):
        //case(ARROW):
        default:
            scanner_.nextLex();
            root = new BinaryOpNode(opLex, root, parseExp(priority + (int)!rightAssocOps[op]));		
            break;
        }
    opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
    }     
    return root;
}

Node* Parser::parseFactor(int priority)
{
	Node* root = NULL;
    bool needNext = true;
	Lexeme* lex = scanner_.get();
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
                scanner_.nextLex();
                OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
				if (!opLex || opLex->getOpType() != PARENTHESIS_FRONT)
					throw parser_exception("Expected open parenthesis", scanner_.getLine(), scanner_.getCol());
                scanner_.nextLex();
				root = new TypecastNode(lex, parseExp());
				opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
				if (!opLex || opLex->getOpType() != PARENTHESIS_BACK)
					throw parser_exception("Expected close parenthesis", scanner_.getLine(), scanner_.getCol());
                scanner_.nextLex();
			} 
			else if (rwType == T_SIZEOF)
			{
				
			}
			else 
				throw parser_exception("Expected typecast word: int(), double(), char() or sizeof()", scanner_.getLine(), scanner_.getCol());
            needNext = false;
			break;
		}
	case(OPERATION):
        {
            OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
            if (*opLex == PARENTHESIS_FRONT)
            {
                scanner_.nextLex();
                root = parseExp();
                Lexeme* lex = scanner_.get();
                OperationLexeme* clLex = dynamic_cast<OperationLexeme*>(lex);
                if(clLex && *clLex != PARENTHESIS_BACK)
                {
                    throw parser_exception ("Expected parenthesis close", false);
                }
            }
            else if (unaryOps[opLex->getOpType()]) 
            {
                scanner_.nextLex();
                root = new UnaryOpNode(lex, parseExp(priorityTable[DEC]));
                needNext = false;
            }
            else if (*opLex == PARENTHESIS_BACK) 
            {
                break;
            }
            else 
            {
                throw parser_exception ("Empty expression is not allowed", false);
            }
        }
	default:
            break;
    }
    if (needNext) {
        scanner_.nextLex();
    }
	return root;
}


Node* Parser::parseFuncCall(Node* root)
{
	Lexeme* lex = scanner_.get();
	Node* r = NULL;
	if (*lex == PARENTHESIS_FRONT)
		{
            scanner_.nextLex();
            Lexeme* l = scanner_.get();
			r = new FuncCallNode(root->lexeme_, root);
            //dynamic_cast<OperationLexeme*>(l) != NULL && 
			while (*l != PARENTHESIS_BACK)
			{
                dynamic_cast<FuncCallNode*>(r)->addArg(parseExp(priorityTable[COMMA] + 1));
				l = scanner_.get();
				if (*l == ENDOF)
					throw parser_exception("Expected parenthesis close after function argument list", scanner_.getCol(), scanner_.getLine());
				if (*l == COMMA){
                    scanner_.nextLex();
				}
			}
		}
	else
		throw parser_exception("Expected PARENTHESIS_FRONT", scanner_.getCol(), scanner_.getLine());
	return r;
}

Node* Parser::parseArrIndex(Node* root)
{
	Node* r = NULL;
	if (*scanner_.get() == BRACKET_FRONT)
		{
			r = new ArrNode(root->lexeme_, root);
			Lexeme* l = scanner_.get();
			while (*l == BRACKET_FRONT)
			{
				Node* k = parseExp();
				dynamic_cast<ArrNode*>(r)->addArg(k);
				l = scanner_.get();
				if (*l == ENDOF)
					throw parser_exception("Expected bracket close after array index", scanner_.getCol(), scanner_.getLine());
			}
		}
	return r;
}