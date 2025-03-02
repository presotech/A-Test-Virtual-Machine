#pragma once

#include "token.h"

struct TokenNode {
    Token token;
    TokenNode *next;
    
    TokenNode(Token t) : token(t), next(NULL) {};
};

struct TokenNodeList {
    TokenNode *head;
    TokenNode *tail;
    int size;
    
    TokenNodeList() : head(NULL), tail(NULL), size(0) {};
};

void append_token(TokenNodeList *list, Token t);
TokenNode *pop_token(TokenNodeList* list);
void destroy_token_list(TokenNodeList *list);

TokenNodeList *create_token_list() {
    auto list = new TokenNodeList;
    
    return list;
}

void append_token(TokenNodeList *list, Token t) {
    auto node = new TokenNode(t);
    
    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
}

TokenNode *pop_token(TokenNodeList* list) {
    if (!list->head) return NULL;
    
    auto node = list->head;
    
    if (list->size == 1) {
        list->head = list->tail = NULL;
        list->size = 0;
    } else {
        auto before_tail = list->head;
        auto cursor = list->head;
        
        while (cursor != list->tail) {
            before_tail = cursor;
            cursor = cursor->next;
        }
        
        list->tail = before_tail;
        before_tail->next = NULL;
    }
    
    list->size--;
    return node;
}

void destroy_token_list(TokenNodeList *list) {
    auto cursor = list->head;
    
    while (cursor) {
        auto current_node = cursor;
        cursor = cursor->next;
        delete current_node;
    }
    
    delete list;
}
