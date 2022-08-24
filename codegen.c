////////////////
//Code generator
////////////////

#include "mycc.h"

//Calculate the address of a variable and push it on stack
void gen_lval(Node *node){
    if(node->kind != ND_LVAR){
        error("left value of assign should be a variable");
    }
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

void gen(Node *node){
    switch(node->kind){
        case ND_NUM:
            printf("    push %d\n", node->val);
            return;
        case ND_LVAR:
            gen_lval(node); //push address of variable on stack
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        case ND_ASSIGN:
            gen_lval(node->lhs);
            gen(node->rhs);
            printf("    pop rdi\n"); //right value of assign
            printf("    pop rax\n"); //address of variable
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n"); //value of assign sentence is the right value
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n"); //Pop the result of rhs to rdi
    printf("    pop rax\n"); //Pop the result of lhs to rax

    switch(node->kind){
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
        case ND_EQ:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NE:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LT:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LE:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
    }

    printf("    push rax\n"); //Store the result on the stack
}

void codegen(){
    //Output assembly
    //Head
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n\n");
    printf("main:\n");

    //Prepare region for variables on stack
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n"); //208 = 8 * 26

    //Convert AST to assembly
    for (int i = 0; code[i]; i++){
        gen(code[i]);

        printf("    pop rax\n"); //Pop result of a expr
    }

    //Result is on the top of the stack
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
    return;
}