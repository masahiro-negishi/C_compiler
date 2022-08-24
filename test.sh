#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    # compile a program that outputs $input, and store the result in $actual
    ./mycc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    # compare $expected and $actual
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

# integer
assert 0 "0;"
assert 42 "42;"

# "+" and "-"
assert 21 "5+20-4;"
assert 110 "0-1+100+11;"

# space
assert 41 " 12 + 34 - 5 ;"

# "*", "/", "(" and ")"
assert 47 "5+6*7;"
assert 15 "5*(9-6);"
assert 4 "(3+5)/2;"
assert 10 "4*5*3/6;"

# unary "+" and "-"
assert 10 "-10+20;"
assert 15 "-3*-5;"

# "==" and "!="
assert 0 "0==1;"
assert 1 "42==42;"
assert 1 "0!=1;"
assert 0 "42!=42;"

# "<", "<=", ">", ">="
assert 1 "0<1;"
assert 0 "1<1;"
assert 0 "2<1;"
assert 1 "0<=1;"
assert 1 "1<=1;"
assert 0 "2<=1;"
assert 1 "1>0;"
assert 0 "1>1;"
assert 0 "1>2;"
assert 1 "1>=0;"
assert 1 "1>=1;"
assert 0 "1>=2;"

# several sentences
assert 4 "2+3; 2+2;"
assert 0 "5*5; 0>1;"

# assign
assert 3 "a=3;"
assert 5 "a=3; z=5;"

echo OK