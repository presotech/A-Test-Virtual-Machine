#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

enum custom_stack_opcodes {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    CMP
};

struct custom_stack_t{
    int count;
    int capacity;
    bool cmp_flag;
    int *values;
    
    custom_stack_t(int _capacity) {
        capacity = _capacity;
        count = 0;
        cmp_flag = false;
        values = (int*)malloc(capacity * sizeof(int));
    };
};

void push(custom_stack_t& stack, int value);
int peek(custom_stack_t& stack);
int pop(custom_stack_t& stack);
void do_op(custom_stack_t& stack, int op_code, int value = -1);
void destroy_stack(custom_stack_t& stack);
void parse_tokens(custom_stack_t& stack, TokenNodeList* list);

int main() {
    custom_stack_t stack(1024);
    
    Lexer lexer("source.asm");
    
    auto tokens = lex_tokens(&lexer);
    
    parse_tokens(stack, tokens);

    destroy_token_list(tokens);
    destroy_lexer(lexer);
    destroy_stack(stack);
    return 0;
}

void parse_tokens(custom_stack_t& stack, TokenNodeList* list) {
    auto cursor = list->head;
    
    while (cursor) {
        switch (cursor->token.type) {
            case TOKEN_PUSH:
                cursor = cursor->next;
                push(stack, atoi(cursor->token.value));
                break;
            case TOKEN_POP:
                pop(stack);
                break;
            case TOKEN_ADD:
                do_op(stack, ADD);
                break;
            case TOKEN_SUB:
                do_op(stack, SUB);
                break;
            case TOKEN_MUL:
                do_op(stack, MUL);
                break;
            case TOKEN_DIV:
                do_op(stack, DIV);
                break;
            case TOKEN_MOD:
                do_op(stack, MOD);
                break;
            case TOKEN_CMP:
                cursor = cursor->next;
                do_op(stack, CMP, atoi(cursor->token.value));
                break;
            case TOKEN_INT:
                continue;
            case TOKEN_PRINT:
                printf("%d\n", peek(stack));
        }
        
        cursor = cursor->next;
    }
}

void push(custom_stack_t& stack, int value) {
    if (stack.count >= stack.capacity) {
        printf("The list is full, making more memory!\n");
        
        stack.capacity *= 2;
        stack.values = (int*)realloc(stack.values, stack.capacity * sizeof(int));
    }
    
    stack.values[stack.count++] = value;
}

int peek(custom_stack_t& stack) {
    if (stack.count == 0) {
        printf("The stack is FUCKING EMPTY!\n");
        return -1;
    }
    
    return stack.values[stack.count - 1];
}

int pop(custom_stack_t& stack) { // we about to pop off now!
    if (stack.count == 0) {
        printf("How the FUCK are we supposed to pop from an EMPTY STACK!?!\n");
        return -1;
    }
    
    return stack.values[--stack.count];
}

void do_op(custom_stack_t& stack, int op_code, int value) {
    int a, b;
    
    if (stack.count < 2 && op_code != CMP) {
        printf("There must be at least two items to perform this action on the stack!\n");
        return;
    }
    
    switch (op_code) {
        case ADD:
            a = pop(stack);
            b = pop(stack);
            
            push(stack, b + a);
            break;
        case SUB:
            a = pop(stack);
            b = pop(stack);
            
            push(stack, b - a);
            break;
        case MUL:
            a = pop(stack);
            b = pop(stack);
            
            push(stack, b * a);
            break;
        case DIV:
            a = pop(stack);
            b = pop(stack);
            
            push(stack, b / a);
            break;
        case MOD:
            a = pop(stack);
            b = pop(stack);
            
            push(stack, b % a);
            break;
        case CMP:
            if (value == -1) {
                printf("No value to compare to!\n");
                return;
            }
            
            if (stack.count < 1) {
                printf("The stack is empty!\n");
                return;
            }
            
            stack.cmp_flag = (peek(stack) == value);
            
            if (stack.cmp_flag) {
                printf("YAY!\n");
            }
            break;
        default:
            printf("Unsupported operation!\n");
            return;
    }
}

void destroy_stack(custom_stack_t& stack) {
    stack.capacity = 0;
    stack.count = 0;
    free(stack.values);
}
