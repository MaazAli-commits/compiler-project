#pragma once
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    // Literals
    NUMBER, IDENTIFIER,
    
    // Operators
    PLUS, MINUS, STAR, SLASH,
    ASSIGN, SEMICOLON, COMMA,
    
    // Comparison
    EQEQ, NEQ, LT, GT, LEQ, GEQ,
    
    // Logical
    AND, OR, NOT,
    
    // Delimiters
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    
    // Keywords
    IF, ELSE, WHILE,
    INT, PRINT, RETURN,
    
    // Special
    END_OF_FILE
};

struct Token {
    TokenType type;
    string value;
    int line;
};

class Lexer {
public:
    Lexer(const string& source);
    vector<Token> tokenize();

private:
    string source;
    int pos;
    int line;

    char current();
    char peek();
    char advance();
    void skipWhitespaceAndComments();
    Token makeNumber();
    Token makeIdentifierOrKeyword();
};