//Compiler

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Token types
typedef enum{
    TK_RESERVED, //Symbols
    TK_NUM, //Integers
    TK_EOF, //End of input
} TokenKind;

typedef struct Token Token;

//Token
struct Token{
    TokenKind kind; //Token type
    Token *next; //Next token
    int val; //Integer value when TokenKind == TK_NUM
    char *str; //String in input that corresponds to the token
};

//Input program
char *user_input;

//Current token
Token *token;

//Raise error
void error(char *fmt, ...){
    va_list ap; //Variable arguments
    va_start(ap, fmt); //Start dealing with variable arguments
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//Show error location when raisng error
void error_at(char *loc, char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");//output #pos spaces
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//When the current token is the same as the expected symbol, move one step.
//Return true when moving one step. Otherwise, return false.
bool consume(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op){
        return false;
    }
    token = token->next;
    return true;
}

//When the current token is the same as the expected symbol, move one step.
//Otherwise, raise error
void expect(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op){
        error_at(token->str, "expected '%c'", op);
    }
    token = token->next;
}

//When the current token is TK_NUM, move one step.
//When moving one step, return the integer value. Otherwise, raise error.
int expect_number(){
    if(token->kind != TK_NUM){
        error_at(token->str, "expected a number");
    }
    int val = token->val;
    token = token->next;
    return val;
}

//Return true when current token is TK_EOF
bool at_eof(){
    return token->kind == TK_EOF;
}

//Make a new token and connect to cur
Token *new_token(TokenKind kind, Token *cur, char *str){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

//Print tokens for debug
void print_tokens(){
    printf("# ");
    Token *tmp = token;
    while(tmp != NULL){
        if(tmp->kind == TK_NUM){
            printf("%d -> ", tmp->val);
        }
        else if(tmp->kind == TK_RESERVED){
            printf("%c -> ", tmp->str[0]);
        }
        else if(tmp->kind == TK_EOF){
            printf("EOF\n");
        }
        tmp = tmp->next;
    }
}

//Tokenize input sequence
Token *tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;//pointer for the last token

    while(*p){
        //Skip space
        if(isspace(*p)){
            p++;
        }
        //Symbol
        else if(*p == '+' || *p == '-'){
            cur = new_token(TK_RESERVED, cur, p++);
        }
        //Integer
        else if(isdigit(*p)){
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
        }
        //Error
        else{
            error_at(p, "Unable to tokenize");
        }
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}


int main(int argc, char **argv){
    //argv[0]: program name
    //argv[1]: input sequence
    if (argc != 2){
        fprintf(stderr, "The number of arguments is incorrect\n");
        return 1;
    }

    //Tokenize
    user_input = argv[1];
    token = tokenize();
    //print_tokens();//for debug

    //Output assembly
    //Head
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n\n");
    printf("main:\n");

    //The type of the first token should be TK_NUM
    printf("    mov rax, %d\n", expect_number());

    //Consume '+ num' or '- num' until reaching EOF
    while(!at_eof()){
        //Add
        if(consume('+')){
            printf("    add rax, %d\n", expect_number());
            continue;
        }
        //Subtract
        expect('-');//Current symbol should be either '+' or '-'
        printf("    sub rax, %d\n", expect_number());           
    }

    printf("    ret\n");
    return 0;
}