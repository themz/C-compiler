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
    st->add(new SymTypeVoid());
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

SymType* Parser::parseType(const parserState state, bool isConst)
{
    string typeName = GL()->getValue();
    if (*GL()!= RESERVEDWORD && *GL() != IDENTIFICATOR) {
        exception("Expected type");
    }
    if (*GL() == T_STRUCT) {
        typeName = parseStruct(state);
    }
    SymType* type = (SymType*)symStack.find(typeName);
    if (type == NULL) {
        exception("Undefine type");
    }
    if(*GL() != IDENTIFICATOR && !type->isTypedef() && *GL() != SEMICOLON)
        NL();
    return type;
}


void Parser::parseDefinition(SymType *type, bool isConst, const parserState state)
{
    Node* exp = NULL;
    type = parseComplexDeclaration(type);
    string name = parseName();
    if (*GL() == PARENTHESIS_FRONT) {
        parseFunctionDeclaration(type, name);
        NL();
    }else if (*GL() == BRACKET_FRONT) {
        addSym(parseArrayDeclaration(type, name, isConst));
    }else if (*GL() == COMMA || *GL() == SEMICOLON) {
        addSym(new SymVar(name, type, exp, isConst));
    }else if (*GL() == ASSIGN){
        NL();
        if (*GL() == BRACES_FRONT) {
            NL();
        }
        exp = parseExp();
        if (exp == NULL) {
            exception("Expected var assign expression");
        }
        if (*GL() == BRACES_BACK) {
            NL();
        }
        addSym(new SymVar(name, type, exp, isConst));
    }else if(*GL() != ASSIGN && isConst && state == PARSE_DEFENITION){
        exception("Default initialization of an object of const type");
    }else{
        exception("Unexpected lexem!");
    }
    if (*GL() == COMMA)
        NL();
}

void Parser::parseDeclaration(const parserState state)
{
    bool isConst = false;
    string name;
    if (*GL() == T_TYPEDEF) {
        parseTypedef();
        return;
    }
    if (*GL() == T_CONST) {
        isConst = true;
        NL();
    }
    SymType *type = parseType(state, isConst);
    if (*GL() == SEMICOLON && type->isStruct() && type->isAnonymousSym()) {
        exception("Anonymous structs must be class members");
    }
    while (*GL() != SEMICOLON) {
        parseDefinition(type, isConst, state);
    }
}


void Parser::addSym(Symbol *symbol){
    if (!symStack.top()->add(symbol)) {
        exception("Redefinition variable \"" + symbol->getName() + "\"");
    }
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

void Parser::parseFunctionDeclaration(SymType *type, string name)
{
    NL();
    SymTable *params = parseFunctionsParams();
    if(*GL() == BRACES_BACK){
        NL();
        NL();
        //parser block
    }
    if (!symStack.top()->add(new SymFunc(name, type, params, NULL))) {
        exception("Redefinition function  \"" + name + "\"");
    }
}

SymTable* Parser::parseFunctionsParams()
{
    SymTable *table = new SymTable();
    SymType *type = NULL;
    string name = "";
    Node *exp = NULL;
    SymVar *var = NULL;
    while (*GL() != PARENTHESIS_BACK) {
        bool isConst = false;
        exp = NULL;
        name = "";
        if (*GL() == T_CONST) {
            NL();
            isConst = true;
        }
        type = parseComplexDeclaration(parseType());
        name = parseName(PARSE_FUNC_ARG_DEF);
        if (*GL() == BRACKET_FRONT) {
            var = parseArrayDeclaration(type, name, isConst);
        } else {
            if(*GL() == ASSIGN)
            {
                NL();
                exp = parseExp(priorityTable[COMMA] + 1);
            }
            var = new SymVar(name, type, exp, isConst);
        }
        if(!table->add(var))
        {
            exception("Redefinition param name:  \"" + name + "\"");
        }
        if (*GL() == COMMA) {
            NL();
        }
    }
    return table;
}

SymVar *Parser::parseArrayDeclaration(SymType *type, string name, bool isConst, bool inFuncDef)
{
    vector<Node *> sizes;
    SymType *arrType = new SymTypeArray(NULL, type);
    Node *value = NULL;
    while (*GL() == BRACKET_FRONT) {
        NL();
        if (*GL() == BRACKET_BACK) {
            NL();
            break;
        }
        sizes.push_back(parseExp());
        if (*GL() != BRACKET_BACK) {
            exception("Expected bracket back after array count");
        }
        NL();
    }
    for (int i = (int)sizes.size() - 1; i >= 0  ; i--) {
        if (i == (int)sizes.size() - 1) {
            dynamic_cast<SymTypeArray *>(arrType)->setSize(sizes[i]);
        } else
            arrType = new SymTypeArray(sizes[i], arrType);
    };
    if (*GL() == ASSIGN) {
        NL();
        NL();
        value = parseExp();
        NL();
    } else if (*GL() == SEPARATOR) {
        if (isConst) {
            exception("Default initialization of an object of const type ");
        } else if(sizes.size() == 0) {
            exception("Definition of variable with array type needs an explicit size or an initializer");
        }
        
    } else if(*GL() == COMMA){
        //
    } else if(*GL() == PARENTHESIS_BACK){
        //NL();
    } else {
        exception("Expected assign or separator");
    }
    return new SymVar(name, arrType ,value, isConst);
}


string Parser::parseStruct(const parserState state)
{
    NL();
    string structName = parseName(PARSE_STRUCT);
    if (*GL() == IDENTIFICATOR && (symStack.find(structName) != NULL)) {
        return structName;
    } else if (*GL() == SEMICOLON || *GL() ==  MULT ||
              (*GL() == IDENTIFICATOR && state == PARSE_TYPEDEF)) {
        addSym(new SymTypeStruct(new SymTable(), structName));
        return structName;
    }
    NL();
    symStack.push(new SymTable());
    while (*GL() != BRACES_BACK) {
        parseDeclaration();
        NL();
    }
    SymTypeStruct *ss = new SymTypeStruct(symStack.top(), structName);
    symStack.pop();
    if (!symStack.top()->add(ss)) {
        exception("Redefinition Struct \"" + structName + "\"");
    }
    return structName;
}

void Parser::parseTypedef()
{
    NL();
    SymTable *table = new SymTable();
    symStack.push(table);
    parseDeclaration(PARSE_TYPEDEF);
    symStack.pop();
    for (Symbol* s : table->table) {
        if (s->isVar()) {
            symStack.add(new SymTypeDef(s->getType(), s->getName()));
        } else if (s->isStruct() && s->getName()[0] != '#')
        {
            symStack.add(s);
        }
    }
}

SymType *Parser::parseComplexDeclaration(SymType *type)
{
    while (*GL() == MULT) {
        type = new SymTypePointer(type);
        NL();
    };
    return type;
}

string Parser::parseName(const parserState state){
    if(*GL() == IDENTIFICATOR) {
        string name = GL()->getValue();
        NL();
        return name;
    } else if ((*GL() == COMMA || *GL() == BRACKET_FRONT || PARENTHESIS_BACK) && state == PARSE_FUNC_ARG_DEF) {
        return "#unname#" + to_string(unnameCount++);
    } else if (state == PARSE_STRUCT && *GL() == BRACES_FRONT) {
        return "#unname#" + to_string(unnameCount++);
    }else{
        exception("Expected identificator" + (string)((state == PARSE_FUNC_ARG_DEF) ? "or no name variable" : ""));
    }
    return "#unname#" + to_string(unnameCount++);
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

