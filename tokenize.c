///////////
//Tokenizer
///////////

#include "mycc.h"

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
    fprintf(stderr, "%*s", pos, " "); //Output #pos spaces
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//When the current token is the same as the expected symbol, move one step.
//Return true when moving one step. Otherwise, return false.
bool consume(char *op){
    if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)){
        return false;
    }
    token = token->next;
    return true;
}

//When the current token is the same as the expected identifier, move one step.
//Return true when moving one step. Otherwise, return false.
Token *consume_ident(){
    if(token->kind != TK_IDENT){
        return NULL;
    }
    Token *t = token;
    token = token->next;
    return t;
}

//When the current token is the same as the expected symbol, move one step.
//Otherwise, raise error
void expect(char *op){
    if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len)){
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
Token *new_token(TokenKind kind, Token *cur, char *str, int len){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
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

//Judge whether p starts with q
bool startswith(char *p, char *q){
    return memcmp(p, q, strlen(q)) == 0;
}

//Tokenize input sequence
Token *tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head; //Pointer for the last token

    while(*p){
        //Skip space
        if(isspace(*p)){
            p++;
        }
        //Multi-letter symbol
        //Multi-letter check should precede Single-letter check
        else if(startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")){
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
        }
        //Single-letter symbol
        else if(strchr("+-*/()<>;=", *p)){
            cur = new_token(TK_RESERVED, cur, p++, 1);
        }
        //Integer
        else if(isdigit(*p)){
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
        }
        //Identifiers
        else if('a' <= *p && *p <= 'z'){
            cur = new_token(TK_IDENT, cur, p++, 1);
        }
        //Error
        else{
            error_at(p, "Unable to tokenize");
        }
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}