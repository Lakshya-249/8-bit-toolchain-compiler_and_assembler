#include "parser.hpp"

Token Parser::consume(TokenType type,std::string message){
    if (!check(type)) {
        error(peek(),message);
    }
    return advance();
}

bool Parser::isAtEnd(){
    // if (peek().type == TokenType::EOFL){
    //     std::cout<<"isAtEnd"<<std::endl;
    // }
    return peek().type == TokenType::EOFL;
}

bool Parser::check(TokenType type){
    if (isAtEnd()) {
        // std::cout<<"this is false check"<<std::endl;
        return false;
    }
    return peek().type == type;
} 

Token Parser::peek(){
    return tokens[current];
}

Token Parser::previous(){
    if (current == 0) return Token(EOFL, "", -1, -1);
    return tokens[current - 1];
}

Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}

bool Parser::match(std::initializer_list<TokenType> tokentypes){
    for(TokenType t: tokentypes){
        if(check(t)){
            advance();
            return true;
        }
    }
    return false;
}

Expr* Parser::expression() {
    return assignment();
}

Expr* Parser::assignment() {
    Expr* expr = orExpr();
    if(match({EQUAL})){
        Token equals = previous();
        Expr* value = assignment();
        if(typeid(*expr) == typeid(Variable)){
            Token name = ((Variable*)expr)->name;
            return new Assign(name, value);
        }
        error(equals, "Invalid assignment target");
        return nullptr;
    }
    return expr;
}

Expr* Parser::orExpr() {
    Expr* expr = andExpr();
    while (match({OR})){
        Token op = previous();
        Expr* right = andExpr();
        expr = new Logical(expr, op, right);
    }
    return expr;
}

Expr* Parser::andExpr() {
    Expr* expr = equality();
    while (match({AND})){
        Token op = previous();
        Expr* right = equality();
        expr = new Logical(expr, op, right);
    }
    return expr;
}

Expr* Parser::equality() {
    Expr* expr = comparision();
    while (match({NOT_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        Expr* right = comparision();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::comparision(){
    Expr* expr = term();
    while (match({LESS, GREATER, GREATER_EQUAL, LESS_EQUAL})){
        Token op = previous();
        Expr* right = term();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::term(){
    Expr* expr = factor();
    while (match({PLUS, MINUS})){
        Token op = previous();
        Expr* right = factor();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::factor(){
    Expr* expr = unary();
    while (match({SLASH,STAR})){
        Token op = previous();
        Expr* right = unary();
        expr = new Binary(expr, op, right);
    }
    return expr;
}

Expr* Parser::unary(){
    if (match({NOT_EQUAL,MINUS,PLUS})){
        Token op = previous();
        Expr* right = unary();
        return new Unary(op, right);
    }
    return primary();
}

Expr* Parser::primary(){
    if (match({FALSE})) return new Literal("0");
    if (match({TRUE})) return new Literal("1");
    if (match({NIL})) return new Literal("0");

    if (match({INTEGER, STRING})) return new Literal(previous().lexeme);

    if (match({IDENTIFIER})) return new Variable(previous());

    if (match({LPAREN})){
        Expr* expr = expression();
        consume(RPAREN, "Expect ')' after expression");
        return expr;
    }
    // error(peek(), "Expect expression.");
    return nullptr;
}

Stmt* Parser::statement() {
    if (match({FOR})) return forStatement();
    if (match({IF})) return ifStatement();
    if (match({PRINT})) return printStatement();
    if (match({WHILE})) return whileStatement();
    if (match({LBRACE})) return new Block(block());
    return exprStatement();
}

Stmt* Parser::ifStatement() {
    consume(LPAREN, "Expect '(' after 'if'");
    Expr* condition = expression();
    consume(RPAREN, "Expect ')' after condition");
    Stmt* thenBranch = statement();
    Stmt* elseBranch = nullptr;
    if (match({ELSE})) {
        elseBranch = statement();
    }
    return new IfStmt(condition, thenBranch, elseBranch);
}

std::vector<Stmt*> Parser::block() {
    std::vector<Stmt*> statements;
    while (!check(RBRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(RBRACE, "Expect '}' after block");
    return statements;
}

Stmt* Parser::printStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after value");
    return new Print(expr);
}

Stmt* Parser::exprStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after expression");
    return new Expression(expr);
}

Stmt* Parser::whileStatement() {
    consume(LPAREN, "Expect '(' after 'while'");
    Expr* condition = expression();
    consume(RPAREN, "Expect ')' after condition");
    Stmt* body = statement();
    return new WhileStmt(condition, body);
}

Stmt* Parser::forStatement() {
    consume(LPAREN, "Expect '(' after 'for'");
    Stmt* initializer;
    if (match({SEMICOLON})) initializer = nullptr;
    else if (match({INT})) initializer = varDeclaration();
    else initializer = exprStatement();

    Expr* condition = nullptr;
    if (!check(SEMICOLON)) condition = expression();
    consume(SEMICOLON, "Expect ';' after condition");

    Expr* increment = nullptr;
    if (!check({RPAREN})) increment = expression();
    consume(RPAREN, "Expect ')' after increment");

    Stmt* body = statement();

    if (increment != nullptr) body = new Block({body, new Expression(increment)});

    if (condition == nullptr) condition = new Literal("1");
    body = new WhileStmt(condition, body);

    if (initializer != nullptr) body = new Block({initializer, body});

    return body;
}

Stmt* Parser::declaration() {
    try{
        if (match({INT})) return varDeclaration();
        return statement();
    }
    catch(const Parser::ParseError &e)
    {
        std::cerr << "Oops an error occured!.." << '\n';
        return nullptr;
    }
    
}

Stmt* Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name");
    Expr* initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }
    consume(SEMICOLON, "Expect ';' after declaration");
    return new Var(name, initializer);
}

std::vector<Stmt*> Parser::parse() {
    try{
        std::vector<Stmt*> statements;
        while (!isAtEnd())
        {
            Stmt* stmt = declaration();
            if (stmt) statements.push_back(stmt);
        }
        return statements;
    }
    catch(const Parser::ParseError &e)
    {
        std::cerr << "Oops an error occured!.." << '\n';
        return {};
    }  
}
