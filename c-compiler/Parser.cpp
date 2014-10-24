#include "Parser.h"

#define NL scanner_.nextLex
#define GL scanner_.get

Parser::Parser(Scanner &scanner):scanner_(scanner), symStack()
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
    NL();
    SymTable *st = new SymTable();
    st->add(new SymTypeInt());
    st->add(new SymTypeChar());
    st->add(new SymTypeFloat());
    symStack.push(st);
}

Node* Parser::parseExp(int priority){
    if (priority > 15)
            return parseFactor();
    Node* l = parseExp(priority + 1);
    Node* root = l;
    Lexeme* lex = GL();
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
                NL();
                break;
            case(PARENTHESIS_FRONT):
                root = parseFuncCall(root);
                break;
            case(BRACKET_FRONT):
                root = parseArrIndex(root);
                NL();
                break;
            case(QUESTION):
            {
                NL();
                Node* l = parseExp();
                Lexeme* le = GL();
                OperationLexeme* sepLex = dynamic_cast<OperationLexeme*>(le);
                if (*sepLex != COLON)
                    throw parser_exception("Missed branch of ternary operator", scanner_.getLine(), scanner_.getCol());
                NL();
                Node* r = parseExp();
                root = new TernaryOpNode(sepLex, root, l, r);
                break;
            }
            case(DOT):
            {
                NL();
                Node* r = parseExp(priority + (int)!rightAssocOps[op]);
                if (r == NULL) {
                    throw parser_exception("Missed branch of binary operator", scanner_.getLine(), scanner_.getCol());
                }
                else if(r->lexeme_->getLexType() != IDENTIFICATOR){
                    throw parser_exception("Expected identificator", scanner_.getLine(), scanner_.getCol());
                }
                root = new BinaryOpNode(opLex, root, r);
                break;
            }
            //case BRACKET_BACK:
            //    root =  NULL;
            //break;
        //case(ARROW):
        default:
                NL();
                BinaryOpNode *bn = new BinaryOpNode(opLex, root, parseExp(priority + (int)!rightAssocOps[op]));
                if (!bn->haveBranch()) {
                    throw parser_exception("Missed branch of binary operator", scanner_.getLine(), scanner_.getCol());
                }
                root = bn;
                break;
        }
    opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
    }     
    return root;
}

SymTable *Parser::parseFuncArg(const bool dec)
{
    SymTable *table = new SymTable();
    Lexeme *l = scanner_.get();
    while (*l != PARENTHESIS_BACK) {
        l = scanner_.get();
        
    }
    return table;
}

SymType* Parser::parseType()
{

    if (*GL()!= RESERVEDWORD && *GL() != IDENTIFICATOR) {
        throw parser_exception("Expected type", scanner_.getLine(), scanner_.getCol());
    }
    if (*GL() == T_STRUCT) {
        //parseStruct()
    }
    SymType* type = (SymType*)symStack.find(GL()->getValue());
    if (type == NULL) {
        throw parser_exception("Undefine type", scanner_.getLine(), scanner_.getCol());
    }
    NL();
    return type;
}

void Parser::parseDeclaration()
{
    Node* exp = NULL;
    bool isConst = false;
    Lexeme* l = GL();
    if (*l == T_CONST) {
        isConst = true;
        NL();
    }
    SymType* type = parseType();
    l = GL();
    while (*GL() != SEPARATOR && *GL() != ENDOF) {
        exp = NULL;
        if (*l == COMMA) {
            NL();
        }
        if (*GL() == MULT) {
            parseComplexDeclaration();
            continue;
            //complex statment
        }
        if (*GL() != IDENTIFICATOR) {
            throw parser_exception("Expected identificator", scanner_.getLine(), scanner_.getCol());
        }
        string name = l->getValue();
        NL();
        if(*GL() == BRACKET_FRONT) {
            parseArrayDeclaration(type, name, isConst);
            if (GL()->getValue() != string("; "))
                break;
            //l = scanner_.get();
            //scanner_.nextLex();
            //exp = parseExp(priorityTable[COMMA] + 1);
            continue;
        }
        if (*GL() != ASSIGN && isConst) {
            throw parser_exception("Default initialization of an object of const type", scanner_.getLine(), scanner_.getCol());
        }
        if (*GL() == ASSIGN) {
            NL();
            exp = parseExp(priorityTable[COMMA] + 1);
            if (exp == NULL) {
                throw parser_exception("Expected var assign expression", scanner_.getLine(), scanner_.getCol());
            }
        }
        else if(isConst){
            throw parser_exception("Default initialization of an object of const type 'const" + type->getName() +"'", scanner_.getLine(), scanner_.getCol());
        }
        if (!symStack.top()->add(new SymVar(l->getValue(), type, exp, isConst))) {
            throw parser_exception("Redefinition variable \"" + l->getValue() + "\"", scanner_.getLine(), scanner_.getCol());
        }

        if (*GL()!= COMMA && *GL() != SEPARATOR && *GL() != ENDOF) {
            throw parser_exception("Expected comma", scanner_.getLine(), scanner_.getCol());
        }
    }
    if (*GL() != SEMICOLON) {
        throw parser_exception("Expected ';' after end of declaration", scanner_.getLine(), scanner_.getCol());
    }
}


void Parser::parseFunction(const bool withBlock)
{
    SymType* type = parseType();
    scanner_.nextLex();
    Lexeme *l = scanner_.get();
    if (*l != IDENTIFICATOR) {
        throw parser_exception("Expected function identificator", scanner_.getLine(), scanner_.getCol());
    }
    scanner_.nextLex();
    if (*scanner_.get() != PARENTHESIS_FRONT) {
        throw parser_exception("Expected '(' ", scanner_.getLine(), scanner_.getCol());
    }
    scanner_.nextLex();
    parseFuncArg();
}

void Parser::parse()
{
    while (*GL() != ENDOF) {
        parseDeclaration();
        NL();
    }

}


Node* Parser::parseFactor(int priority)
{
	Node* root = NULL;
    bool needNext = true;
	Lexeme* lex = GL();
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
	case(FLOAT):
		root = new FloatNode(lex);
		break;
	case(CHAR):
		root = new CharNode(lex);
        break;
	case RESERVEDWORD:
		{
			ReservedWordType rwType = dynamic_cast<ReservedWordLexeme*>(lex)->getRwType();
			if (rwType == T_CHAR || rwType == T_INT || rwType == T_FLOAT)
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
                NL();
			} 
			else if (rwType == T_SIZEOF)
			{
				
			}
			else 
				throw parser_exception("Expected typecast word: int(), float(), char() or sizeof()", scanner_.getLine(), scanner_.getCol());
            needNext = false;
			break;
		}
	case(OPERATION):
        {
            OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(lex);
            if (*opLex == PARENTHESIS_FRONT)
            {
                NL();
                root = parseExp();
                Lexeme* lex = GL();
                if(!lex || *lex != PARENTHESIS_BACK)
                {
                    throw parser_exception ("Expected parenthesis close", false);
                }
            }
            else if (unaryOps[opLex->getOpType()]) 
            {
                NL();
                root = new UnaryOpNode(lex, parseExp(priorityTable[DEC]));
                needNext = false;
            }
            else if (*opLex == PARENTHESIS_BACK) 
            {
                throw parser_exception ("Expected parenthesis open", scanner_.getLine(), scanner_.getCol());
                break;
            }
            else if (*opLex == BRACKET_FRONT)
            {
                NL();
                root = parseExp();
                needNext = false;
            }
//            else 
//            {
//                throw parser_exception ("Empty expression is not allowed", false);
//            }
            break;
        }
	default:
            break;
    }
    if (needNext) {
        NL();
    }
	return root;
}

void Parser::parseArrayDeclaration(SymType *type, string name, bool isConst)
{
    NL();
    Node *size = NULL, *value = NULL;
    if (*GL() != BRACKET_BACK) {
         size = parseExp();
    } else {
        size = NULL;
    }
    if (*GL() != BRACKET_BACK) {
        throw parser_exception ("Expected bracket back after array count", scanner_.getLine(), scanner_.getCol());
    }
    NL();
    if (*GL() == ASSIGN) {
        NL();
        NL();
        value = parseExp();
    } else if (*GL() == SEPARATOR) {
        if (isConst) {
            throw parser_exception ("Default initialization of an object of const type ", scanner_.getLine(), scanner_.getCol());
        };
    } else {
        throw parser_exception ("Expected assign or separator", scanner_.getLine(), scanner_.getCol());
    }
    symStack.top()->add(new SymVar(name, new SymTypeArray(size, type),value, isConst));
}

void Parser::parseComplexDeclaration()
{
    Lexeme *l = scanner_.get();
    while (*l != SEPARATOR && *l != COMMA) {
        scanner_.nextLex();
        l = scanner_.get();
        if (*l == ENDOF) {
            throw parser_exception ("Expected bracket back after array count", scanner_.getLine(), scanner_.getCol());
        }
        
    }
    
    
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
            scanner_.nextLex();
		}
	else
		throw parser_exception("Expected PARENTHESIS_FRONT", scanner_.getCol(), scanner_.getLine());
	return r;
}

Node* Parser::parseArrIndex(Node* root)
{
	Node* r = NULL;
	if (*GL() == BRACKET_FRONT)
		{
            r = (root == NULL) ? new ArrNode(NULL, root) : new ArrNode(root->lexeme_, root);
            NL();
            dynamic_cast<ArrNode*>(r)->addArg(parseExp());
            if (*GL() != BRACKET_BACK)
                throw parser_exception("Expected bracket close after array index", scanner_.getCol(), scanner_.getLine());
        }
	return r;
}

void Parser::printTable()
{
    symStack.print();
}

