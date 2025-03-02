#pragma once

#include <stdio.h>

enum TokenType {
    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_CMP,
    TOKEN_INT,
    TOKEN_MOD,
    TOKEN_PRINT
};

struct Token {
    TokenType type;
    char* value;
    
    Token(TokenType t, char* v) : type(t), value(v) {};
};
