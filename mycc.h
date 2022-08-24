#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////
//tokenize.c
////////////

//Token kinds
typedef enum{
    TK_RESERVED, //Symbols
    TK_IDENT, //Identifiers
    TK_NUM, //Integers
    TK_EOF, //End of input
} TokenKind;

//Token type
typedef struct Token Token;
struct Token{
    TokenKind kind; //Token kind
    Token *next; //Next token
    int val; //Integer value when TokenKind == TK_NUM
    char *str; //String in input that corresponds to the token
    int len; //Length of token (length of string for TK_RESERVED, number of digits for TK_NUM, 0 for TK_EOF)
};

//Raise error
void error(char *fmt, ...);

//Show error location when raisng error
void error_at(char *loc, char *fmt, ...);

//When the current token is the same as the expected symbol, move one step.
//Return true when moving one step. Otherwise, return false.
bool consume(char *op);

//When the current token is the same as the expected identifier, move one step.
//Return true when moving one step. Otherwise, return false.
Token *consume_ident();

//When the current token is the same as the expected symbol, move one step.
//Otherwise, raise error
void expect(char *op);

//When the current token is TK_NUM, move one step.
//When moving one step, return the integer value. Otherwise, raise error.
int expect_number();

//Return true when current token is TK_EOF
bool at_eof();

//Make a new token and connect to cur
Token *new_token(TokenKind kind, Token *cur, char *str, int len);

//Tokenize input sequence
Token *tokenize();

//for debug
void print_tokens();

//Input program
extern char *user_input;

//Current token
extern Token *token;

/////////
//parse.c
/////////

//Node kinds
typedef enum{
    ND_ADD, //"+"
    ND_SUB, //"-"
    ND_MUL, //"*"
    ND_DIV, //"/"
    ND_EQ, //"=="
    ND_NE, //"!="
    ND_LT, //"<"
    ND_LE, //"<="
    ND_ASSIGN, //"="
    ND_LVAR, //Local variables
    ND_NUM, //Integer
} NodeKind;

//AST node type
typedef struct Node Node;
struct Node{
    NodeKind kind; //Node kind
    Node *lhs; //Left-hand side
    Node *rhs; //Right-hand side
    int val; //Integer value when kind == ND_NUM
    int offset; //offset from base pointer when kind == ND_LVAR
};

//Store several sentences
extern Node *code[100];

//recursive descent parsing
void program();

///////////
//codegen.c
///////////

//generate code
void codegen();