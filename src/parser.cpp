#include "parser.h"
#include <stdexcept>

using namespace std;

Parser::Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}

Token Parser::current() {
    return tokens[pos];
}

Token Parser::peek() {
    if (pos + 1 < (int)tokens.size()) return tokens[pos + 1];
    return tokens.back();
}

Token Parser::advance() {
    return tokens[pos++];
}

Token Parser::expect(TokenType type) {
    if (current().type != type)
        throw runtime_error("Unexpected token: " + current().value + " on line " + to_string(current().line));
    return advance();
}

bool Parser::check(TokenType type) {
    return current().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) { advance(); return true; }
    return false;
}

// Parse entire program as list of statements
vector<ASTNodePtr> Parser::parseProgram() {
    vector<ASTNodePtr> statements;
    while (!check(TokenType::END_OF_FILE)) {
        statements.push_back(parseStatement());
    }
    return statements;
}

ASTNodePtr Parser::parseStatement() {
    if (check(TokenType::INT))    return parseVarDecl();
    if (check(TokenType::PRINT))  return parsePrint();
    if (check(TokenType::IF))     return parseIf();
    if (check(TokenType::WHILE))  return parseWhile();
    return parseAssignment();
}

// int x = 5;
ASTNodePtr Parser::parseVarDecl() {
    expect(TokenType::INT);
    string name = expect(TokenType::IDENTIFIER).value;
    expect(TokenType::ASSIGN);
    ASTNodePtr init = parseExpr();
    expect(TokenType::SEMICOLON);
    return make_unique<VarDeclNode>(name, move(init));
}

// x = 5;
ASTNodePtr Parser::parseAssignment() {
    string name = expect(TokenType::IDENTIFIER).value;
    expect(TokenType::ASSIGN);
    ASTNodePtr val = parseExpr();
    expect(TokenType::SEMICOLON);
    return make_unique<AssignNode>(name, move(val));
}

// print(x);
ASTNodePtr Parser::parsePrint() {
    expect(TokenType::PRINT);
    expect(TokenType::LPAREN);
    ASTNodePtr expr = parseExpr();
    expect(TokenType::RPAREN);
    expect(TokenType::SEMICOLON);
    return make_unique<PrintNode>(move(expr));
}

// if (cond) { ... } else { ... }
ASTNodePtr Parser::parseIf() {
    expect(TokenType::IF);
    expect(TokenType::LPAREN);
    ASTNodePtr cond = parseExpr();
    expect(TokenType::RPAREN);
    vector<ASTNodePtr> thenBlock = parseBlock();
    vector<ASTNodePtr> elseBlock;
    if (match(TokenType::ELSE)) {
        elseBlock = parseBlock();
    }
    return make_unique<IfNode>(move(cond), move(thenBlock), move(elseBlock));
}

// while (cond) { ... }
ASTNodePtr Parser::parseWhile() {
    expect(TokenType::WHILE);
    expect(TokenType::LPAREN);
    ASTNodePtr cond = parseExpr();
    expect(TokenType::RPAREN);
    vector<ASTNodePtr> body = parseBlock();
    return make_unique<WhileNode>(move(cond), move(body));
}

// { statement; statement; }
vector<ASTNodePtr> Parser::parseBlock() {
    expect(TokenType::LBRACE);
    vector<ASTNodePtr> stmts;
    while (!check(TokenType::RBRACE) && !check(TokenType::END_OF_FILE)) {
        stmts.push_back(parseStatement());
    }
    expect(TokenType::RBRACE);
    return stmts;
}

// Handles == != < > <= >=
ASTNodePtr Parser::parseExpr() {
    return parseComparison();
}

ASTNodePtr Parser::parseComparison() {
    ASTNodePtr left = parseTerm();
    while (check(TokenType::EQEQ) || check(TokenType::NEQ) ||
           check(TokenType::LT)   || check(TokenType::GT)  ||
           check(TokenType::LEQ)  || check(TokenType::GEQ)) {
        string op = advance().value;
        ASTNodePtr right = parseTerm();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

// Handles + -
ASTNodePtr Parser::parseTerm() {
    ASTNodePtr left = parseFactor();
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        string op = advance().value;
        ASTNodePtr right = parseFactor();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

// Handles * /
ASTNodePtr Parser::parseFactor() {
    ASTNodePtr left = parsePrimary();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        string op = advance().value;
        ASTNodePtr right = parsePrimary();
        left = make_unique<BinaryOpNode>(op, move(left), move(right));
    }
    return left;
}

// Handles numbers, variables, parentheses
ASTNodePtr Parser::parsePrimary() {
    if (check(TokenType::NUMBER)) {
        int val = stoi(advance().value);
        return make_unique<NumberNode>(val);
    }
    if (check(TokenType::IDENTIFIER)) {
        string name = advance().value;
        return make_unique<VariableNode>(name);
    }
    if (match(TokenType::LPAREN)) {
        ASTNodePtr expr = parseExpr();
        expect(TokenType::RPAREN);
        return expr;
    }
    throw runtime_error("Unexpected token in expression: " + current().value);
}