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

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 110 "0-1+100+11"

echo OK