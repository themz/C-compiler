#include "Parser.h"

#define NL scanner_.nextLex
#define GL scanner_.get

typedef enum {
    PARSE_FUNC_ARG_DEF,
    PARSE_FUNC_ARG,
    PARSE_STRUCT,
} parserState;

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
                    exception("Missed branch of ternary operator");
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
                    exception("Missed branch of binary operator");
                }
                else if(r->lexeme_->getLexType() != IDENTIFICATOR){
                    exception("Expected identificator");
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
                    exception("Missed branch of binary operator");
                }
                root = bn;
                break;
        }
    opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
    }     
    return root;
}

SymType* Parser::parseType()
{
    if (*GL()!= RESERVEDWORD && *GL() != IDENTIFICATOR) {
        exception("Expected type");
    }
    if (*GL() == T_STRUCT) {
        //parseStruct()
    }
    SymType* type = (SymType*)symStack.find(GL()->getValue());
    if (*GL() == T_VOID) {
        type = new SymTypeVoid();
    }
    if (type == NULL) {
        exception("Undefine type");
    }
    NL();
    return type;
}



void Parser::parseDeclaration()
{
    Node* exp = NULL;
    bool isConst = false;
    string name;
    if (*GL() == T_CONST) {
        isConst = true;
        NL();
    }
    if (*GL() == T_STRUCT) {
        parseStruct();
        return;
    }
    SymType* type = parseType();
    while (*GL() != SEPARATOR && *GL() != ENDOF) {
        exp = NULL;
        if (*GL() == COMMA) {
            NL();
        }
        if (*GL() == MULT) {
            parseComplexDeclaration();
            continue;
            //complex statment
        }
        if (*GL() != IDENTIFICATOR) {
            exception("Expected identificator");
        }
        name = GL()->getValue();
        NL();
        if(*GL() == BRACKET_FRONT) {
            parseArrayDeclaration(type, name, isConst);
            NL();
            continue;
        } else if(*GL() == PARENTHESIS_FRONT) {
            parseFunctions(type, name);
            return;
        }
        if (*GL() != ASSIGN && isConst) {
            exception("Default initialization of an object of const type");
        }
        if (*GL() == ASSIGN) {
            NL();
            exp = parseExp(priorityTable[COMMA] + 1);
            if (exp == NULL) {
                exception("Expected var assign expression");
            }
        }
        else if(isConst){
            exception("Default initialization of an object of const type 'const" + type->getName() +"'");
        }
        if (!symStack.top()->add(new SymVar(name, type, exp, isConst))) {
            exception("Redefinition variable \"" + name + "\"");
        }
        if (*GL()!= COMMA && *GL() != SEPARATOR && *GL() != ENDOF) {
            exception("Expected comma");
        }
    }
    if (*GL() != SEMICOLON && *GL() != ENDOF) {
        exception("Expected ';' after end of declaration");
    }
}

void Parser::parse()
{
    while (*GL() != ENDOF) {
        parseDeclaration();
        NL();
        if (*GL() == SEMICOLON) {
            NL();
        }
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
					exception("Expected open parenthesis");
                scanner_.nextLex();
				root = new TypecastNode(lex, parseExp());
				opLex = dynamic_cast<OperationLexeme*>(scanner_.get());
				if (!opLex || opLex->getOpType() != PARENTHESIS_BACK)
					exception("Expected close parenthesis");
                NL();
			} 
			else if (rwType == T_SIZEOF)
			{
				
			}
			else 
				exception("Expected typecast word: int(), float(), char() or sizeof()");
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
                    exception("Expected parenthesis close");
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
                exception("Expected parenthesis open");
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

void Parser::parseFunctions(SymType *type, string name)
{
    NL();
    SymType *varType;
    Node *exp;
    bool defArg = false;
    bool isConstVar = false;
    SymTable *args = new SymTable();
    while(*GL() != SEPARATOR && *GL() != ENDOF && *GL() != PARENTHESIS_BACK)
    {
        isConstVar = false;
        if (*GL() == T_CONST) {
            isConstVar = true;
            NL();
        }
        exp = NULL;
        varType = parseType();
        if (varType == NULL) {
            exception("Expected var type");
        }
        if (*GL() == COMMA || *GL() == PARENTHESIS_BACK) {
            args->add(new SymVar("", varType, exp, isConstVar));
            NL();
            continue;
        } else if (*GL() != IDENTIFICATOR) {
            exception("Expected identificator");
        }
        string name = GL()->getValue();
        NL();
        if (*GL() == ASSIGN) {
            defArg = true;
            NL();
            exp = parseExp(priorityTable[COMMA] + 1);
            NL();
        } else if(defArg){
            exception("Missing default argument on parameter \"" + name + "\"");
        } else if(*GL() == COMMA){
            NL();
        }
        args->add(new SymVar(name, varType, exp, isConstVar));
    }
    if(*GL() == BRACES_BACK){
        NL();
        NL();
        //parser block
    }
    if (!symStack.top()->add(new SymFunc(name, type, args, NULL))) {
        exception("Redefinition function  \"" + name + "\"");
    }
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
        exception("Expected bracket back after array count");
    }
    NL();
    if (*GL() == ASSIGN) {
        NL();
        NL();
        value = parseExp();
    } else if (*GL() == SEPARATOR) {
        if (isConst) {
            exception("Default initialization of an object of const type ");
        };
    } else if(*GL() == COMMA){
        //
    } else {
        exception("Expected assign or separator");
    }
    symStack.top()->add(new SymVar(name, new SymTypeArray(size, type),value, isConst));
}

void Parser::parseStruct()
{
    NL();
    string name = GL()->getValue();
    NL();
    NL();
    symStack.push(new SymTable());
    while (*GL() != BRACES_BACK) {
        parseDeclaration();
        NL();
    }
    SymTypeStruct *ss = new SymTypeStruct(symStack.top(), name);
    symStack.pop();
    if (!symStack.top()->add(ss)) {
        exception("Redefinition Struct \"" + name + "\"");
    }

}

void Parser::parseComplexDeclaration()
{
    Lexeme *l = scanner_.get();
    while (*l != SEPARATOR && *l != COMMA) {
        scanner_.nextLex();
        l = scanner_.get();
        if (*l == ENDOF) {
            exception("Expected bracket back after array count");
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
					exception("Expected parenthesis close after function argument list");
				if (*l == COMMA){
                    scanner_.nextLex();
				}
			}
            scanner_.nextLex();
		}
	else
        exception("Expected PARENTHESIS_FRONT");
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
                exception("Expected bracket close after array index");
        }
	return r;
}

void Parser::printTable(int deep)
{
    symStack.print(deep);
}

void Parser::exception(string msg)
{
    throw parser_exception(msg, scanner_.getCol(), scanner_.getLine());
}


