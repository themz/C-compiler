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
    


    SymTable *st = new SymTable();
    st->add(new SymTypeInt());
    st->add(new SymTypeChar());
    st->add(new SymTypeFloat());
    st->add(new SymTypeVoid());
    symStack.push(st);
    
    names = new nameStack();
    state = new stateStack();
    state->push(PARSE_START);
    
    NL();
}


void Parser::parse()
{
    while (*GL() != ENDOF) {
        parseExternalDecl();
        NL();
    }
}


//======================= Expression

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
                SymType *type = root->getType();
                exception("Left operand of '.' must be a struct ", !dynamic_cast<SymTypeStruct*>(type));
                exception("Expected identificator", *GL() != IDENTIFICATOR);
                Symbol *member = dynamic_cast<SymTypeStruct*>(type)->find(GL()->getValue());
                exception("No member named '" + GL()->getValue() + "' in struct '" + type->getName() + "'", member == NULL);
                root = new BinaryOpNode(opLex, root, new IdentifierNode(GL(), (SymVar*)member));
                NL();
                break;
            }
            case(ARROW):
            {
                NL();
                SymType *type = root->getType();
                exception("Left operand of '.' must be a pointer to struct ", !dynamic_cast<SymTypePointer*>(type));
                SymType *StructType = dynamic_cast<SymTypePointer*>(type)->getType();
                exception("Left operand of '.' must be a pointer to struct ", !dynamic_cast<SymTypeStruct*>(StructType));
                exception("Expected identificator", *GL() != IDENTIFICATOR);
                Symbol *member = dynamic_cast<SymTypeStruct*>(StructType)->find(GL()->getValue());
                exception("No member named '" + GL()->getValue() + "' in struct '" + StructType->getName() + "'", member == NULL);
                root = new BinaryOpNode(opLex, root, new IdentifierNode(GL(), (SymVar*)member));
                NL();
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
        {
            SymVar *v = NULL;
            if (cState == PARSE_EXP_SYM) {
                v = (SymVar*)symStack.find(lex->getValue());
                exception("Use of undeclared identifier '" + lex->getValue() + "'", v == NULL);
                exception("'" + lex->getValue() + "' is not variable", v->Symbol::isVar());
            }
            root = new IdentifierNode(lex, v);
            break;
        }
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
                exception("Expected parenthesis open",  state->top() != PARSE_CYCLE && state->top() != PARSE_IF);
                break;
            }
            else if (*opLex == BRACKET_FRONT)
            {
                NL();
                root = parseExp();
                needNext = false;
            }
            else if (*opLex == COMMA)
            {
               
            }
            break;
        }
        case (SEPARATOR):
        {
            needNext = false;
        }
        default:
            break;
    }
    if (needNext) {
        NL();
    }
    return root;
}


//======================= Declarator


void Parser::parseExternalDecl()
{
    parseSemicolon();
    if (*GL() == ENDOF) {
        return;
    }
    bool isConst = false;
    string name;
    if (*GL() == T_TYPEDEF) {
        exception("Duplicate 'typedef' declaration specifier", state->top() == PARSE_TYPEDEF);
        state->push(PARSE_TYPEDEF);
        parseTypedef();
        state->pop();
        return;
    }
    if (*GL() == T_CONST) {
        isConst = true;
        NL();
    }
    SymType *type = NULL;
    SymType *lastType = parseType(isConst);
    exception("Anonymous structs must be class members", *GL() == SEMICOLON && lastType->isStruct() && lastType->isAnonymousSym());
    while (*GL() != SEMICOLON && *GL() != BRACE_BACK && *GL() != ENDOF) {
        SymType* complexType = parseDeclarator();
        type = hitch(complexType, lastType);
        string name = names->pop();
        exception("Variable has incomplete type 'struct " + type->getName() + "'", type->isStruct() && type->isEmpty() && state->top() != PARSE_TYPEDEF);
        if (type->isFunc()) {
            dynamic_cast<SymFunc*>(type)->setName(name);
            symStack.add(type);
        } else {
            SymVar* var = new SymVar(name, type, NULL, isConst);
            if (*GL() == ASSIGN) {
                parseInitializer(var);
                if (*GL() == COMMA) {
                    NL();
                }
            }
            symStack.add(var);
            
        }
        if (*GL() == COMMA && state->top() == PARSE_FUNC_ARG_DEF) {
            break;
        }
    }
}

SymType* Parser::parseDeclarator()
{
    SymType* newType = parseDirectDeclarator(NULL);
    exception("Expected ';' or ',' or '=' or EOF  not '" + GL()->getValue() + "'",
              *GL() != SEMICOLON && *GL() != COMMA && *GL() != BRACE_BACK && *GL() != ASSIGN && *GL() != ENDOF && (*GL() != PARENTHESIS_BACK && state->top() == PARSE_FUNC_ARG_DEF));
    if (*GL() == COMMA && state->top() != PARSE_FUNC_ARG_DEF) {
        NL();
    }
    return newType;
}

SymType* Parser::parseType(bool isConst)
{
    string typeName = GL()->getValue();
    exception("Expected type", *GL()!= RESERVEDWORD && *GL() != IDENTIFICATOR);
    if (*GL() == T_STRUCT) {
        typeName = parseStruct();
    }
    SymType* type = (SymType*)symStack.find(typeName);
    exception("Undefine type", type == NULL || type->isFunc());
    if (!type->isStruct()){
        NL();
    }
    return type;
}

SymType* Parser::parseDirectDeclarator(SymType *type)
{
    SymType *getFromRecType = NULL;
    type = parsePointerDeclaration(type);
    
    if(*GL() == PARENTHESIS_FRONT)
    {
        NL();
        getFromRecType = parseDirectDeclarator(NULL);
    } else {
        if (state->top() == PARSE_FUNC_ARG_DEF && *GL() != IDENTIFICATOR) {
            names->push("#unname#" + to_string(unnameCount++));
        }else{
            exception("Expected identificator", *GL() != IDENTIFICATOR);
            exception("Redefinition variable '" + GL()->getValue() + "'",
                          symStack.top()->find(GL()->getValue()) != NULL);
            //if (*GL() != IDENTIFICATOR) {
            //    names->push("#unname#" + to_string(unnameCount++));
            ///} else{
            names->push(GL()->getValue());
            NL();
        }
    }
    SymType* ok = NULL;
    while (*GL() == BRACKET_FRONT || *GL() == PARENTHESIS_FRONT) {
        if(*GL() == BRACKET_FRONT) {
            ok = parseArrayDeclaration(ok);
        }else if(*GL() ==  PARENTHESIS_FRONT) {
            ok = parseFunctionDeclaration(ok);
        } else if(*GL() == PARENTHESIS_BACK) {
            NL();
            //break;
        } else{
            exception("Unexpected lex in declarate '"+ GL()->getValue() +"'");
        }
    }
    ok = revers(ok);
    type = hitch(ok, type);
    if (*GL() == PARENTHESIS_BACK && state->top() != PARSE_FUNC_ARG_DEF) {
        NL();
    }
    return hitch(getFromRecType, type);
}

SymType* Parser::revers(SymType *head)
{
    SymType *r = NULL;
    while (head != NULL) {
        SymType *s = head;
        head = head->getType();
        s->setType(r);
        r = s;
    }
    return r;
}

SymType* Parser::hitch(SymType* start, SymType* type)
{
    if (start == NULL) {
        return type;
    }
    SymType* last = start;
    while (last->getType() != NULL) {
        last = last->getType();
    }
    last->setType(type);
    return start;
}

void Parser::addSym(Symbol *symbol){
    if (!symStack.top()->add(symbol)) {
        exception("Redefinition variable \"" + symbol->getName() + "\"");
    }
}

SymType *Parser::parseFunctionDeclaration(SymType *type, bool parseParam)
{
    NL();
    StmtBlock *body = NULL;
    SymTable *params = parseFunctionsParams();
    if(*GL() == BRACE_FRONT){
        exception("Expected ')' in function declaration can not have body", parseParam);
        exception("Parameters name omitted", params->hasAnonymousSym());
        body = parseBlock(params);
    } else if(*GL() == SEMICOLON || *GL() == COMMA || *GL() == BRACKET_FRONT || *GL() == PARENTHESIS_BACK || *GL() == PARENTHESIS_FRONT){
        
    }
    else {
        exception("Expected function body, complexDeclaration or ';' but not '" + GL()->getValue() + "'");
    }
    string s = "";
    return new SymFunc(s, type, params, body);
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
    symStack.push(table);
    while (*GL() != PARENTHESIS_BACK) {
        bool isConst = false;
        if (*GL() == T_CONST) {
            NL();
            isConst = true;
        }
        state->push(PARSE_FUNC_ARG_DEF);
        SymType *type = parseType();
        SymType* complpexType = parseDeclarator();
        type = hitch(complpexType, type) ;
        state->pop();
        string name = names->pop();
        exception("C does not support default arguments", *GL() == ASSIGN);
        exception("Redefinition param name:  \"" + name + "\"", !table->add(new SymVar(name, type, NULL, isConst, true)));
        exception("Expected ',' or identificator", *GL() != COMMA && *GL() != PARENTHESIS_BACK);
        if (*GL() == COMMA) {
            NL();
        }
    }
    NL();
    symStack.pop();
    return table;
}

//SymTable* Parser::parseFunctionsParams()
//{
//    SymTable *table = new SymTable();
//    SymType *type = NULL;
//    string name = "";
//    Node *exp = NULL;
//    while (*GL() != PARENTHESIS_BACK) {
//        bool isConst = false;
//        exp = NULL;
//        name = "";
//        if (*GL() == T_CONST) {
//            NL();
//            isConst = true;
//        }
//        type = parsePointerDeclaration(parseType());
//        state->push(PARSE_FUNC_ARG_DEF);
//        name = parseName();
//        state->pop();
//        if (*GL() == BRACKET_FRONT) {
//            type = parseArrayDeclaration(type);
//        }
//        exception("C does not support default arguments", *GL() == ASSIGN);
//        exception("Redefinition param name:  \"" + name + "\"", !table->add(new SymVar(name, type, exp, isConst, true)));
//        exception("Expected ',' or identificator", *GL() != COMMA && *GL() != PARENTHESIS_BACK);
//        if (*GL() == COMMA) {
//            NL();
//        }
//    }
//    NL();
//    return table;
//}

SymType *Parser::parseArrayDeclaration(SymType *type)
{
    vector<Node *> sizes;
    SymType *arrType = type;
    new SymTypeArray(NULL, type);
    while (*GL() == BRACKET_FRONT) {
        NL();
        if (*GL() == BRACKET_BACK) {
            arrType = new SymTypeArray(new EmptyNode(), arrType);
            NL();
            break;
        }
        arrType = new SymTypeArray(parseExp(), arrType);
        exception("Expected bracket back after array count", *GL() != BRACKET_BACK);
        NL();
    }
    return arrType;
}

string Parser::parseStruct()
{
    NL();
    state->push(PARSE_STRUCT);
    exception("Declaration of anonymous struct must be a definition", *GL() != IDENTIFICATOR && *GL() != BRACE_FRONT);
    string structName = parseName();
    if (symStack.find('$' + structName) == NULL) {
        symStack.add(new SymTypeStruct(NULL, '$' + structName));
    }
    if(*GL() == BRACE_FRONT) {
        NL();
        symStack.push(new SymTable());
        while (*GL() != BRACE_BACK) {
            parseExternalDecl();
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
    state->pop();
    return structName;
}

void Parser::parseTypedef()
{
    NL();
    SymTable *table = new SymTable();
    symStack.push(table);
    state->push(PARSE_TYPEDEF);
    parseExternalDecl();
    state->pop();
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
    NL();
    exception("Expected expression", *GL() == SEMICOLON);
    ListNode *list = new ListNode(NULL, NULL);
    while (*GL() != BRACE_BACK) {
        if (*GL() == BRACE_FRONT) {
            list->addArg(parseInitList());
            if (*GL() == COMMA) {
                NL();
            }
            continue;
        }
        list->addArg(parseExp(priorityTable[COMMA] + 1));
        exception("Expected ',' or } or {", *GL() != COMMA && *GL() != BRACE_FRONT && *GL() != BRACE_BACK);
        if (*GL() == COMMA) {
            NL();
        }
    }
    NL();
    return list;
}

void Parser::parseInitializer(SymVar *var)
{
    NL();
    Node *val =  *GL() == BRACE_FRONT ? parseInitList() : parseExp(priorityTable[COMMA] + 1);
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

string Parser::parseName(){
    if(*GL() == IDENTIFICATOR) {
        string name = GL()->getValue();
        NL();
        return name;
    } else if ((*GL() == COMMA || *GL() == BRACKET_FRONT || PARENTHESIS_BACK) && state->top() == PARSE_FUNC_ARG_DEF) {
        return "#unname#" + to_string(unnameCount++);
    } else if (state->top() == PARSE_STRUCT && *GL() == BRACE_FRONT) {
        return "#unname#" + to_string(unnameCount++);
    }else{
        exception("Expected identificator" + (string)((state->top() == PARSE_FUNC_ARG_DEF) ? "or no name variable" : ""));
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
    r = new ArrNode(root->lexeme_, root,root->getType());
    NL();
    Node *idx = parseExp();
    exception("Array subscript is not an integer", !dynamic_cast<SymTypeInt*>(idx->getType()));
    dynamic_cast<ArrNode*>(r)->addIndex(idx);
    exception("Expected bracket close after array index", *GL() != BRACKET_BACK);
    return r;
}


//----------Stmt

StmtBlock *Parser::parseBlock(SymTable* table)
{
    if (*GL() == BRACE_FRONT) {
        NL();
    }
    vector<Stmt*> statements;
    StmtBlock *block = new StmtBlock();
    symStack.push(table != NULL ? table : new SymTable());
    while (*GL() != BRACE_BACK && *GL() != ENDOF) {
        Symbol *s = symStack.find(GL()->getValue());
        if ((s != NULL && s->isType()) || *GL() == T_TYPEDEF || *GL() == T_STRUCT) {
            parseExternalDecl();
            NL();
        } else {
            Stmt *s = parseStmt();
            if (s != NULL) {
                block->addStmt(s);
            }
//            if( *GL() == BRACE_FRONT
//               || *GL() == T_DO
//               || *GL() == T_WHILE
//               || *GL() == T_FOR
//               || *GL() == T_IF
//               || *GL() == T_BREAK
//               || *GL() == T_CONTINUE
//               || *GL() == T_BREAK
//               || *GL() == T_RETURN
//               || *GL() == SEMICOLON)
//            {
//                break;
//            }
        }
    }
    if (*GL() == BRACE_BACK) {
        NL();
    }
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
    state->push(PARSE_IF);
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
    state->pop();
    return new StmtIf(con, trueBlock, falseBlock);
}

Stmt *Parser::parseFor()
{
    state->push(PARSE_CYCLE);
    Stmt *body = NULL;
    NL();
    exception("Expected open parenthesis", *GL() != PARENTHESIS_FRONT);
    NL();
    Node *ini = parseExp();
    NL();
    Node *con = parseExp();
    NL();
    Node *inc = NULL;
    if (*GL() != PARENTHESIS_BACK) {
         inc = parseExp();
    }
    if (*GL() != PARENTHESIS_BACK) {
        exception("Expected ')'");
    }
    NL();
    if(*GL() == SEMICOLON) {
        NL();
        
//    } else if (*GL() == BRACE_FRONT
//               || *GL() == T_DO
//               || *GL() == T_WHILE
//               || *GL() == T_FOR
//               || *GL() == T_IF
//               || *GL() == T_BREAK
//               || *GL() == T_CONTINUE
//               || *GL() == T_BREAK
//               || *GL() == T_RETURN) {
//        body = parseBlock();
    } else {

        body = parseBlock();
        //exception("Unexpected lexeme '" + GL()->getValue() + "'");
    }
    state->pop();
    return new StmtFor(ini, con, inc, body);
}

Stmt *Parser::parseWhile()
{
    state->push(PARSE_CYCLE);
    NL();
    Node *cond = parseCondition();
    NL();
    state->pop();
    return new StmtWhile(cond, parseBlock());
    
}

Stmt *Parser::parseDoWhile()
{
    state->push(PARSE_CYCLE);
    NL();
    Stmt* body = parseBlock();
    exception("Expected while", *GL() != T_WHILE);
    Node *con = parseCondition();
    state->pop();
    return new StmtDoWhile(con, body);
}

Stmt *Parser::parseJumpStatement()
{
    Stmt * statment = NULL;
    if (*GL() == T_BREAK) {
        exception("'break' statement not in loop statement", state->find(PARSE_CYCLE));
        statment = new StmtBreak();
        NL();
    } else if(*GL() == T_CONTINUE) {
        exception("'continue' statement not in loop statement", state->find(PARSE_CYCLE));
        statment = new StmtContinue();
        NL();
    } else if (*GL() == T_RETURN){
        exception("'return' statement not in function block statement", state->find(PARSE_CYCLE));
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

//------------------Helper

string nameStack::pop()
{
    string name = names.back();
    names.pop_back();
    return name;
}

void nameStack::push(string name)
{
    names.push_back(name);
}

parserState stateStack::pop()
{
    parserState state = states.back();
    states.pop_back();
    return state;
}

parserState stateStack::top()
{
    return states.back();
}

void stateStack::push(parserState state)
{
    states.push_back(state);
}

bool stateStack::find(parserState state)
{
    for (int i = 0; i < states.size(); i++) {
        if (states[i] == state) {
            return true;
        }
    }
    return false;
}


