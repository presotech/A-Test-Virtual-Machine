#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "nodelist.h"

char *read_entire_file(const char* filename);

struct Lexer {
    int pos;
    char *source_code;
    size_t count;

    Lexer(const char* filename) : pos(0), source_code(read_entire_file(filename)), count(strlen(source_code)) {}
};

TokenNodeList *lex_tokens(Lexer* lexer) {
    auto token_list = new TokenNodeList;

    if (lexer->source_code == NULL) {
        printf("There is no source code loaded into the lexer, it returned null.\n");
        delete token_list;
        return NULL;
    }

    int BUFFER_SIZE = 1024;
    auto buffer = (char*)malloc(BUFFER_SIZE);
    size_t current_buffer_length = 0;
    int index = 0;

    while (index < lexer->count) {
        if (lexer->source_code[index] == ' ' || lexer->source_code[index] == '\n' ||
            lexer->source_code[index] == '\r' || lexer->source_code[index] == '\t') {index++; continue;}

        if (current_buffer_length + 1 >= BUFFER_SIZE) {
            BUFFER_SIZE *= 2;
            buffer = (char*)realloc(buffer, BUFFER_SIZE);
        }

        if (isalpha(lexer->source_code[index])) {
            while (isalpha(lexer->source_code[index])) {
                buffer[current_buffer_length++] = lexer->source_code[index++];
            }
            buffer[current_buffer_length] = '\0';

            if (strcmp(buffer, "push") == 0) {
                append_token(token_list, {TOKEN_PUSH, NULL});
            }

            if (strcmp(buffer, "pop") == 0) {
                append_token(token_list, {TOKEN_POP, NULL});
            }

            if (strcmp(buffer, "add") == 0) {
                append_token(token_list, {TOKEN_ADD, NULL});
            }

            if (strcmp(buffer, "print") == 0) {
                append_token(token_list, {TOKEN_PRINT, NULL});
            }

            if (strcmp(buffer, "cmp") == 0) {
                append_token(token_list, {TOKEN_CMP, NULL});
            }

            if (strcmp(buffer, "mod") == 0) {
                append_token(token_list, {TOKEN_MOD, NULL});
            }

            current_buffer_length = 0;
            buffer[0] = '\0';
            continue;
        }

        if (isdigit(lexer->source_code[index])) {
            while (isdigit(lexer->source_code[index])) {
                buffer[current_buffer_length++] = lexer->source_code[index++];
            }
            buffer[current_buffer_length] = '\0';

            append_token(token_list, {TOKEN_INT, strdup(buffer)});
            current_buffer_length = 0;
            buffer[0] = '\0';
            continue;
        }
    }

    free(buffer);
    return token_list;
}

void destroy_lexer(Lexer& lexer) {
    lexer.pos = 0;
    lexer.count = 0;
    delete[] lexer.source_code;
}

char *read_entire_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: We could not open %s!\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size == -1) {
        printf("We could not get the size of the file!\n");
        fclose(file);
        return NULL;
    }

    char *buffer = new char[file_size + 1];
    size_t bytes_read = fread(buffer, 1, file_size, file);

    if (bytes_read < file_size) {
        printf("All bytes could not be read!\n");
        delete[] buffer;
        fclose(file);
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}
