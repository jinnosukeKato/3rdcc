#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./3rdcc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 1 "1 != 0"
assert 0 "2 * 5 != 10"
assert 1 "1 <= 1"
assert 0 "2 <= 1"
assert 1 "5 > 0"
assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34  - 5 "
assert 24 "3 * 8"
assert 123 "5 * 20+23"
assert 9 "81/9"
assert 5 "28/5"
assert 10 "2 +8 / 1"
assert 47 "(1+3)*12-1"
assert 39 "+39"
assert 22 "-11*-2"
assert 1 "1 == 1"
assert 0 "40 == 2"
assert 1 "5 * 2 == 10"

echo OK
