#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>

using namespace std;

class Parser {
public:
    Parser(const vector<Token>& tokens);
    vector<ASTNodePtr> parseProgram();

private:
    vector<Token> tokens;
    int pos;

    Token current();
    Token peek();
    Token advance();
    Token expect(TokenType type);
    bool check(TokenType type);
    bool match(TokenType type);

    ASTNodePtr parseStatement();
    ASTNodePtr parseVarDecl();
    ASTNodePtr parseAssignment();
    ASTNodePtr parsePrint();
    ASTNodePtr parseIf();
    ASTNodePtr parseWhile();
    vector<ASTNodePtr> parseBlock();

    ASTNodePtr parseExpr();
    ASTNodePtr parseComparison();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
    ASTNodePtr parsePrimary();
};