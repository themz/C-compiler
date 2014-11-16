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
    if (GL() == NULL || *GL() == ENDOF || *GL() == PARENTHESIS_BACK ||
        *GL() == BRACKET_BACK || *GL() == COLON || *GL() == SEPARATOR) {
            return root;
    }
    OperationLexeme* opLex = dynamic_cast<OperationLexeme*>(GL());
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
                if (*GL() != COLON)
                    exception("Missed branch of ternary operator");
                NL();
                Node* r = parseExp();
                root = new TernaryOpNode(GL(), root, l, r);
                break;
            }
            case(DOT):
            {
                NL();
                exception("Left expression must be identificator", *root->lexeme_ != IDENTIFICATOR);
                SymVar *strct = (SymVar*)symStack.find(root->lexeme_->getValue());
                exception("Use of undeclared identifier '" + root->lexeme_->getValue() + "'", strct == NULL);
                if (!strct->getType()->isStruct()){
                    exception("Member '"+ strct->getName() +"'  reference base type '"
                              + strct->getType()->getName() + "' is not a structure ");
                }
                Node* r = parseExp(priority + (int)!rightAssocOps[op]);
                exception("Expected identificator", r->lexeme_->getLexType() != IDENTIFICATOR);
                Symbol *member = ((SymTypeStruct*)strct->getType())->find(r->lexeme_->getValue());
                exception("No member named '" + r->lexeme_->getValue() + "' in struct '$" + strct->getName() + "'", member == NULL);
                root = new BinaryOpNode(opLex, root, r);
                break;
            }
            case(ARROW):
            {
                NL();
//                if (*root->lexeme_ != IDENTIFICATOR) {
//                    exception("Left expression must be identificator");
//                }
//                SymVar *strct = (SymVar*)symStack.find(root->lexeme_->getValue());
//                if(strct == NULL) {
//                    exception("Use of undeclared identifier '" + root->lexeme_->getValue() + "'");
//                } else if (!strct->getType()->isStruct()){
//                    exception("Member '"+ strct->getName() +"'  reference base type '"
//                              + strct->getType()->getName() + "' is not a structure ");
//                }
//                Node* r = parseExp(priority + (int)!rightAssocOps[op]);
//                if(r->lexeme_->getLexType() != IDENTIFICATOR){
//                    exception("Expected identificator");
//                }
//                Symbol *member = ((SymTypeStruct*)strct->getType())->find(r->lexeme_->getValue());
//                if(member == NULL) {
//                    exception("No member named '" + r->lexeme_->getValue() + "' in struct '$" + strct->getName() + "'");
//                }
//                root = new BinaryOpNode(opLex, root, r);
                break;
            }
        default:
                NL();
                BinaryOpNode *bn = new BinaryOpNode(opLex, root, parseExp(priority + (int)!rightAssocOps[op]));
                exception("Missed branch of binary operator", !bn->haveBranch());
                root = bn;
                break;
        }
    opLex = dynamic_cast<OperationLexeme*>(GL());
    }     
    return root;
}

SymType* Parser::parseType(const parserState state, bool isConst)
{
    string typeName = GL()->getValue();
    exception("Expected type", *GL()!= RESERVEDWORD && *GL() != IDENTIFICATOR);
    if (*GL() == T_STRUCT) {
        typeName = parseStruct(state);
    }
    SymType* type = (SymType*)symStack.find(typeName);
    exception("Undefine type", type == NULL);
    if (!type->isStruct()){
        NL();
    }
    return type;
}

SymVar* Parser::parseDirectDeclarator(SymType *type, bool isConst, const parserState state)
{
    SymVar *var = NULL;
    if(*GL() == PARENTHESIS_FRONT)
    {
        NL();
        return parseDeclarator(type, isConst);
    } else {
        exception("Expected identificator", *GL() != IDENTIFICATOR);
        exception("Redifinition variable '" + GL()->getValue() + "'",
                  symStack.top()->find(GL()->getValue()) != NULL);
    }
    var = new SymVar(GL()->getValue(), type, NULL, isConst);
    NL();
    if (*GL() == PARENTHESIS_FRONT) {
        parseFunctionDeclaration(type, var->getName());
        NL();
        return NULL;
    }else if(*GL() == BRACKET_FRONT) {
        var->setType(parseArrayDeclaration(var->getType()));
    }
    if(*GL() == ASSIGN)
    {
        parseInitializer(var);
    }
    if (*GL() == COMMA) {
        NL();
    }
    return var;
}

SymVar* Parser::parseDeclarator(SymType *type, bool isConst, const parserState state)
{
    return parseDirectDeclarator(parsePointerDeclaration(type), isConst, state);
}

void Parser::parseTypeSpec(const parserState state)
{
    parseSemicolon();
    if (*GL() == ENDOF) {
        return;
    }
    bool isConst = false;
    string name;
    if (*GL() == T_TYPEDEF) {
        exception("Duplicate 'typedef' declaration specifier", state == PARSE_TYPEDEF);
        parseTypedef();
        return;
    }
    if (*GL() == T_CONST) {
        isConst = true;
        NL();
    }
    SymType *type = parseType(state, isConst);
        exception("Anonymous structs must be class members",
                  *GL() == SEMICOLON && type->isStruct() && type->isAnonymousSym());
#warning Убрать BRACE_BACK и ENDOF
    while (*GL() != SEMICOLON && *GL() != BRACE_BACK && *GL() != ENDOF) {
            symStack.add(parseDeclarator(type, isConst, state));
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
        parseTypeSpec();
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

			if (*lex == T_CHAR || *lex == T_INT || *lex == T_FLOAT)
			{
                NL();
                exception("Expected open parenthesis", *GL() != PARENTHESIS_FRONT);
                NL();
				root = new TypecastNode(lex, parseExp());
                exception("Expected close parenthesis", *GL() != PARENTHESIS_BACK);
                NL();
			} 
			else if (*lex == T_SIZEOF)
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
                exception("Expected parenthesis close", *GL() != PARENTHESIS_BACK);
            }
            else if (unaryOps[opLex->getOpType()]) 
            {
                NL();
                root = new UnaryOpNode(lex, parseExp(priorityTable[DEC]));
                needNext = false;
            }
            else if (*opLex == PARENTHESIS_BACK) 
            {
                exception("Expected parenthesis open", pState != PARSE_CYCLE && pState != PARSE_IF);
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
    parserState p = pState;
    pState = PARSE_FUNC;
    NL();
    StmtBlock *body = NULL;
    SymTable *params = parseFunctionsParams();
    if(*GL() == BRACE_FRONT){
        body = parseBlock();
    } else if(*GL() == SEMICOLON || *GL() == COMMA){
        //parseSemicolon();
    } else {
        exception("Expected function body or ;");
    }
    if (!symStack.top()->add(new SymFunc(name, type, params, body))) {
        exception("Redefinition function  \"" + name + "\"");
    }
    pState = p;
}

void Parser::parseSemicolon()
{
    while (*GL() == SEMICOLON) {
        NL();
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
        type = parsePointerDeclaration(parseType());
        name = parseName(PARSE_FUNC_ARG_DEF);
        if (*GL() == BRACKET_FRONT) {
            var->setType(parseArrayDeclaration(type));
        } else {
            if(*GL() == ASSIGN)
            {
                NL();
                exp = parseExp(priorityTable[COMMA] + 1);
            }
            var = new SymVar(name, type, exp, isConst);
        }
        exception("Redefinition param name:  \"" + name + "\"", !table->add(var));
        if (*GL() == COMMA) {
            NL();
        }
    }
    NL();
    return table;
}

SymType *Parser::parseArrayDeclaration(SymType *type)
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
        exception("Expected bracket back after array count", *GL() != BRACKET_BACK);
        NL();
    }
    for (int i = (int)sizes.size() - 1; i >= 0  ; i--) {
        if (i == (int)sizes.size() - 1) {
            dynamic_cast<SymTypeArray *>(arrType)->setSize(sizes[i]);
        } else
            arrType = new SymTypeArray(sizes[i], arrType);
    };
    return arrType;
}


string Parser::parseStruct(const parserState state)
{
    NL();
    exception("Declaration of anonymous struct must be a definition", *GL() != IDENTIFICATOR && *GL() != BRACE_FRONT);
    string structName = parseName(PARSE_STRUCT);
    if (symStack.find('$' + structName) == NULL) {
        symStack.add(new SymTypeStruct(NULL, '$' + structName));
    }
    if(*GL() == BRACE_FRONT) {
        NL();
        symStack.push(new SymTable());
        while (*GL() != BRACE_BACK) {
            parseTypeSpec();
            NL();
        }
        SymTable* table = symStack.top();
        symStack.pop();
        Symbol *str = symStack.top()->find('$' + structName );
        if (str != NULL) {
            if (str->isStruct()) {
                if(str->isEmpty()){
                    dynamic_cast<SymTypeStruct *>(str)->setTable(table);
                } else {
                    exception("Redefinition Struct \"" + structName + "\"");
                }
            } else {
                symStack.add(new SymTypeStruct(table, '$' + structName));
            }
        } else {
            symStack.add(new SymTypeStruct(table, '$' + structName));
        }
        NL();
    }
    return structName;
}

void Parser::parseTypedef()
{
    NL();
    SymTable *table = new SymTable();
    symStack.push(table);
    parseTypeSpec(PARSE_TYPEDEF);
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

Node *Parser::parseInitList()
{
    int deep = 0;
    BinaryOpNode *op = NULL;
    Node *exp = NULL;
    exception("Expected expression", *GL() == SEMICOLON);
    while (*GL() != SEMICOLON && *GL() !=ENDOF) {
        if ( *GL() == BRACE_FRONT) {
            deep++;
            NL();
            continue;
        } else if ( *GL() == BRACE_BACK) {
            deep--;
            NL();
            if (deep == 0) {
                break;
            }
            continue;
        } else if(*GL() == COMMA)
        {
            NL();
            continue;
        }
        exp = parseExp(priorityTable[COMMA] + 1);
        exception("Expected expression", exp == NULL);
        op = new BinaryOpNode(new OperationLexeme(scanner_.getLine(),scanner_.getCol(), "," ,SEPARATOR, COMMA), op, exp);
    }
    exception("Expected ';' at end of declaration ", *GL() != SEMICOLON);
    exception("Imbalance brace ",deep != 0);
    return op;
}

void Parser::parseInitializer(SymVar *var)
{
    NL();
    Node *val = parseInitList();
    SymType *type = var->getType();
    exception("scalar initializer cannot be empty", val == NULL && !type->isStruct() && !type->isArray());
    var->setVal(val);
    
}

SymType *Parser::parsePointerDeclaration(SymType *type)
{
    while (*GL() == MULT) {
        NL();
        type = new SymTypePointer(type, *GL() == T_CONST);
        while (*GL() == T_CONST) {
            NL();
        }
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
    } else if (state == PARSE_STRUCT && *GL() == BRACE_FRONT) {
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
                exception("Expected parenthesis close after function argument list", *l == ENDOF);
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
            exception("Expected bracket close after array index", *GL() != BRACKET_BACK);
        }
	return r;
}


//----------Stmt

StmtBlock *Parser::parseBlock()
{
    NL();
    vector<Stmt*> statements;
    StmtBlock *block = new StmtBlock();
    symStack.push(new SymTable());
    while (*GL() != BRACE_BACK && *GL() != ENDOF) {
        Symbol *s = symStack.find(GL()->getValue());
        if ((s != NULL && s->isType()) || *GL() == T_TYPEDEF || *GL() == T_STRUCT) {
            parseTypeSpec();
            NL();
        } else {
            Stmt *s = parseStmt();
            if (s != NULL) {
                block->addStmt(s);
            }
        }
    }
    NL();
    block->setSymTable(symStack.top());
    symStack.pop();
    return block;
}

Node *Parser::parseCondition()
{
    NL();
    Node *con = parseExp();
    exception("Expected condition!", con == NULL);
    return con;
}

Stmt *Parser::parseStmt()
{
    if (*GL() == T_IF)
        return parseIf();
    else if (*GL() == T_FOR)
        return parseFor();
    else if (*GL() == T_WHILE)
        return parseWhile();
    else if (*GL() == T_DO)
        return parseDoWhile();
    else if (*GL() == BRACE_FRONT)
        return (Stmt*)parseBlock();
    else if (*GL() == T_CONTINUE || *GL() == T_BREAK || *GL() == T_RETURN)
        return parseJumpStatement();
    else {
        Node *exp = parseExp();
        StmtSingle* statement = NULL;
        if(exp != NULL)
            statement = new StmtSingle(exp);
        NL();
        return statement;
    }
}

Stmt *Parser::parseIf()
{
    parserState p = pState;
    pState = PARSE_IF;
    NL();
    Node *con = parseCondition();
    NL();
    Stmt *trueBlock = parseStmt();
    Stmt *falseBlock = NULL;
    if (*GL() == T_ELSE)
    {
        NL();
        falseBlock = parseStmt();
    }
    pState = p;
    return new StmtIf(con, trueBlock, falseBlock);
}

Stmt *Parser::parseFor()
{
    parserState p = pState;
    pState = PARSE_CYCLE;
    Stmt *body = NULL;
    NL();
    exception("Expected open parenthesis", *GL() != PARENTHESIS_FRONT);
    NL();
    Node *ini = parseExp();
    NL();
    Node *con = parseExp();
    NL();
    Node *inc = parseExp();
    NL();
    if(*GL() == SEMICOLON) {
        
    } else if (*GL() == BRACE_FRONT) {
        body = parseBlock();
    } else {
        exception("Expected { or ;");
    }
    pState = p;
    return new StmtFor(ini, con, inc, body);
}

Stmt *Parser::parseWhile()
{
    parserState p = pState;
    pState = PARSE_CYCLE;
    NL();
    pState = p;
    return new StmtWhile(parseCondition(), parseBlock());

}

Stmt *Parser::parseDoWhile()
{
    parserState p = pState;
    pState = PARSE_CYCLE;
    NL();
    Stmt* body = parseBlock();
    exception("Expected while", *GL() != T_WHILE);
    Node *con = parseCondition();
    pState = p;
    return new StmtDoWhile(con, body);
}

Stmt *Parser::parseJumpStatement()
{
    Stmt * statment = NULL;
    if (*GL() == T_BREAK) {
        exception("'break' statement not in loop statement", pState != PARSE_CYCLE);
        statment = new StmtBreak();
        NL();
    } else if(*GL() == T_CONTINUE) {
        exception("'continue' statement not in loop statement", pState != PARSE_CYCLE);
        statment = new StmtContinue();
        NL();
    } else if (*GL() == T_RETURN){
        exception("'return' statement not in function block statement", pState != PARSE_FUNC);
        NL();
        Node *exp = parseExp();
        statment = new StmtReturn(exp);
    }
    exception("Expected ;", *GL() != SEMICOLON);
    NL();
    return statment;
}

void Parser::printTable(int deep)
{
    symStack.print(deep);
}

void Parser::exception(string msg, bool cond)
{
    if (cond)
        throw parser_exception(msg, scanner_.getCol(), scanner_.getLine());
}

