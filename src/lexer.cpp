#include "lexer.h"
#include <stdexcept>
#include <cctype>

using namespace std;

Lexer::Lexer(const string& source) 
    : source(source), pos(0), line(1) {}

char Lexer::current() {
    return pos < (int)source.size() ? source[pos] : '\0';
}

char Lexer::peek() {
    return (pos + 1) < (int)source.size() ? source[pos + 1] : '\0';
}

char Lexer::advance() {
    return source[pos++];
}

void Lexer::skipWhitespaceAndComments() {
    while (pos < (int)source.size()) {
        if (current() == '\n') { line++; pos++; }
        else if (isspace(current())) { pos++; }
        else if (current() == '/' && peek() == '/') {
            while (current() != '\n' && pos < (int)source.size()) pos++;
        }
        else break;
    }
}

Token Lexer::makeNumber() {
    string num;
    while (isdigit(current())) num += advance();
    return {TokenType::NUMBER, num, line};
}

Token Lexer::makeIdentifierOrKeyword() {
    string word;
    while (isalnum(current()) || current() == '_') word += advance();
    
    if (word == "if")     return {TokenType::IF,     word, line};
    if (word == "else")   return {TokenType::ELSE,   word, line};
    if (word == "while")  return {TokenType::WHILE,  word, line};
    if (word == "int")    return {TokenType::INT,     word, line};
    if (word == "print")  return {TokenType::PRINT,  word, line};
    if (word == "return") return {TokenType::RETURN, word, line};
    
    return {TokenType::IDENTIFIER, word, line};
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    
    while (pos < (int)source.size()) {
        skipWhitespaceAndComments();
        if (pos >= (int)source.size()) break;
        
        char c = current();
        
        if (isdigit(c))          { tokens.push_back(makeNumber());              continue; }
        if (isalpha(c) || c=='_'){ tokens.push_back(makeIdentifierOrKeyword()); continue; }
        
        switch (c) {
            case '+': tokens.push_back({TokenType::PLUS,      "+", line}); advance(); break;
            case '-': tokens.push_back({TokenType::MINUS,     "-", line}); advance(); break;
            case '*': tokens.push_back({TokenType::STAR,      "*", line}); advance(); break;
            case '/': tokens.push_back({TokenType::SLASH,     "/", line}); advance(); break;
            case ';': tokens.push_back({TokenType::SEMICOLON, ";", line}); advance(); break;
            case ',': tokens.push_back({TokenType::COMMA,     ",", line}); advance(); break;
            case '(': tokens.push_back({TokenType::LPAREN,    "(", line}); advance(); break;
            case ')': tokens.push_back({TokenType::RPAREN,    ")", line}); advance(); break;
            case '{': tokens.push_back({TokenType::LBRACE,    "{", line}); advance(); break;
            case '}': tokens.push_back({TokenType::RBRACE,    "}", line}); advance(); break;
            case '=':
                if (peek() == '=') { tokens.push_back({TokenType::EQEQ,   "==", line}); advance(); advance(); }
                else               { tokens.push_back({TokenType::ASSIGN,  "=",  line}); advance(); }
                break;
            case '!':
                if (peek() == '=') { tokens.push_back({TokenType::NEQ,    "!=", line}); advance(); advance(); }
                else               { tokens.push_back({TokenType::NOT,    "!",  line}); advance(); }
                break;
            case '<':
                if (peek() == '=') { tokens.push_back({TokenType::LEQ,    "<=", line}); advance(); advance(); }
                else               { tokens.push_back({TokenType::LT,     "<",  line}); advance(); }
                break;
            case '>':
                if (peek() == '=') { tokens.push_back({TokenType::GEQ,    ">=", line}); advance(); advance(); }
                else               { tokens.push_back({TokenType::GT,     ">",  line}); advance(); }
                break;
            case '&':
                if (peek() == '&') { tokens.push_back({TokenType::AND,    "&&", line}); advance(); advance(); }
                else throw runtime_error("Unknown token '&'");
                break;
            case '|':
                if (peek() == '|') { tokens.push_back({TokenType::OR,     "||", line}); advance(); advance(); }
                else throw runtime_error("Unknown token '|'");
                break;
            default:
                throw runtime_error(string("Unknown character: ") + c);
        }
    }
    
    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}