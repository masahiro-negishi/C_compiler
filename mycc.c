//compiler

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    //argv[0]: program name
    //argv[1]: number to output
    if (argc != 2){
        fprintf(stderr, "The number of arguments is incorrect\n");
        return 1;
    }

    char *p = argv[1];

    //output assembly
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n\n");
    printf("main:\n");
    printf("    mov rax, %ld\n", strtol(p, &p, 10));

    while(*p){
        if(*p == '+'){
            p++;
            printf("    add rax, %ld\n", strtol(p, &p, 10));
        }
        else if(*p == '-'){
            p++;
            printf("    sub rax, %ld\n", strtol(p, &p, 10));           
        }
        else{
            fprintf(stderr, "Unexpected character: '%c'\n", *p);
            return 1;
        }
    }

    printf("    ret\n");
    return 0;
}