# Bool Tool
A CLI parser and evaluator for boolean expressions, written in C99 without external dependencies.
Prints the truth table of arbitrary propositional formulas to your terminal.

### Syntax
##### Operators
| Operator | Syntax |
| -- | -- |
| TRUE | 1 |
| FALSE | 0 |
| NOT | ! |
| AND | & |
| OR | \| |
| IMPLICATION | -> |
| EQUIVALENCE | = |

With precedence as the table suggests.
Note that the implication operator is right associative, but for clarity's sake it is best not to rely on that and use parenthesis instead.

##### Variables
The program can handle up to 64 arbitrary length variables. Variable names are alphanumerical, case sensitive and can contain underscores, but must start with an alphabetic character.


### Usage

Upon executing the program, you will be greeted with a prompt. Simply enter your formula and you will receive a truth table.

An example:

    Enter your expression:
    (a & b) -> c

     a | b | c |
    ---------------
     0 | 0 | 0 | 1
     0 | 0 | 1 | 1
     0 | 1 | 0 | 1
     0 | 1 | 1 | 1
     1 | 0 | 0 | 1
     1 | 0 | 1 | 1
     1 | 1 | 0 | 0
     1 | 1 | 1 | 1

### Building
Quite straight forward, move into the root directory of the project and do:

		make build
		cd build
		cmake ..
		make

### Inner workings

A quick overview of how this works.

#### Lexing and Parsing

The character stream of the expression is first processed by a lexer and split into a stream of tokens.
Eg an input of `(a | b) & c` is processed to `[ PAR_OPEN "(", VARIABLE "a", OR "|", VARIABLE "b", PAR_CLOSE ")", AND "&", VARIABLE "c" ]`.
This reduces the input stream to information that is relevant for the next parsing step, eg "noise" such as whitespace is removed.
See `src/lex.c` for implementation details.

Those tokens are then parsed with a handwritten recursive descent parser on the basis of following grammar rules:
```
<expr> ::= <impl> "="  <expr> | <impl>
<impl> ::= <disj> "->" <impl> | <disj>
<disj> ::= <conj> "|" <disj> | <conj>
<conj> ::= <nega> "&" <conj> | <nega>
<nega> ::= "!" <nega> | <atom>
<atom> ::= "1" | "0"  | [a-z][a-z0-9_]* | "(" <expr> ")"
```

Recursive descent parsers are quite convenient to implement since each rule can be implemented as one function. The resulting set 
of functions thus reads much like the grammar defined above (see `src/parse.c`).
The parser builds a nice, easy to work with syntax tree for us.

Our example from above, `(a | b) & c` (or rather, its lexed token stream), is parsed to

```
[AND, "&&"]
  [OR, "||"]
    [VARIABLE, "a"]
    [VARIABLE, "b"]
  [VARIABLE, "c"]
 ```

As you can see, this tree now also encodes the structure of the expression, operation precedence and so on.

#### Evaluating

Once the syntax tree is built, evaluation is rather trivial. The tree is stepped through recursively and evaluated at each node.
In pseudo code:
```
fn eval (node):
	match node.type:
		AND 		=> eval(node.left_child) && eval(node.right_child)
		OR  		=> eval(node.left_child) || eval(node.right_child)
		/* ... */
		VARIABLE 	=> get_variable_truth_assignment(node.variable_name)
		TRUE		=> 1
		FALSE		=> 0
```

See the exact implementation at https://github.com/bubba2k/bool-tool/blob/1211244a827ed50e3cddaa17909af063fd5f18dc/src/formula.c#L82

Building the truth table is then just a matter of evaluating the expression tree for all possible truth assignments of the variables
in the expression: 

```
a | b | c |
---------------
 0 | 0 | 0 | 0
 0 | 0 | 1 | 0
 0 | 1 | 0 | 0
 0 | 1 | 1 | 1
 1 | 0 | 0 | 0
 1 | 0 | 1 | 1
 1 | 1 | 0 | 0
 1 | 1 | 1 | 1
```

