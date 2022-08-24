///////////////////////////
//Parser
//Recursive Descent Parsing
///////////////////////////

#include "mycc.h"

//Create a new node
//kind != ND_NUM
Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

//Create a new node
//kind == ND_NUM
Node *new_node_num(int val){
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

//Store several sentences
Node *code[100];

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

//program = stmt*
void program(){
    int i = 0;
    while(!at_eof()){
        code[i] = stmt();
        i++;
    }
    code[i] = NULL;
}

//stmt = expr ";"
Node *stmt(){
    Node *node = expr();
    expect(";");
    return node;
}

//expr = assign
Node *expr(){
    return assign();
}

//assign = equality ("=" equality) ?
Node *assign(){
    Node *node = equality();
    if(consume("=")){
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

//equality = relational ("==" relational | "!=" relational)*
Node *equality(){
    Node *node = relational();
    while(1){
        if(consume("==")){
            node = new_node(ND_EQ, node, relational());
        }
        else if(consume("!=")){
            node = new_node(ND_NE, node, relational());
        }
        else{
            return node;
        }
    }
}

//relational = add ("<" add | "<=" add | ">" add | ">= add")*
Node *relational(){
    Node *node = add();
    while(1){
        if(consume("<")){
            node = new_node(ND_LT, node, add());
        }
        else if(consume("<=")){
            node = new_node(ND_LE, node, add());
        }
        else if(consume(">")){
            node = new_node(ND_LT, add(), node);
        }
        else if(consume(">=")){
            node = new_node(ND_LE, add(), node);
        }
        else{
            return node;
        }
    }
}


//add = mul ("+" mul | "-" mul)*
Node *add(){
    Node *node = mul();
    while(1){
        if(consume("+")){
            node = new_node(ND_ADD, node, mul());
        }
        else if(consume("-")){
            node = new_node(ND_SUB, node, mul());
        }
        else{
            return node;
        }
    }
}

//mul = unary ("*" unary | "/" unary)*
Node *mul(){
    Node *node = unary();
    while(1){
        if(consume("*")){
            node = new_node(ND_MUL, node, unary());
        }
        else if(consume("/")){
            node = new_node(ND_DIV, node, unary());
        }
        else{
            return node;
        }
    }
}

//unary = ("+" | "-")? primary
Node *unary(){
    if(consume("+")){
        return unary(); //Convert +x to x
    }
    else if(consume("-")){
        return new_node(ND_SUB, new_node_num(0), primary()); //Convert -x to 0-x
    }
    else{
        return primary();
    }
}

//primary = num | ident | "(" expr ")"
Node *primary(){
    if(consume("(")){
        Node *node = expr();
        expect(")");
        return node;
    }

    //ident
    Token *tok = consume_ident();
    if(tok){
        //calculate offset
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        node->offset = (tok->str[0] - 'a' + 1) * 8; 
        return node;
    }

    //num
    return new_node_num(expect_number());
}