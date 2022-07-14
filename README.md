# logic-parser
A small CLI program that parses boolean expressions and prints their truth tables.

Educational project for exploring BNF and recursive descent parsing.

Example output:
    Enter your expression:
    a && b -> c

     a | b | c |
    ---------------
     0 | 0 | 0 | 1
     1 | 0 | 0 | 1
     0 | 1 | 0 | 1
     1 | 1 | 0 | 0
     0 | 0 | 1 | 1
     1 | 0 | 1 | 1
     0 | 1 | 1 | 1
     1 | 1 | 1 | 1
