//Compiler

#include "mycc.h"

int main(int argc, char **argv){
    //argv[0]: program name
    //argv[1]: input sequence
    if (argc != 2){
        error("The number of arguments is incorrect");
        return 1;
    }

    //Tokenize
    user_input = argv[1];
    token = tokenize();
    //print_tokens();//for debug

    //Generate Abstract Syntax Tree(AST)
    program();

    //Output assembly
    codegen();
    return 0;
}