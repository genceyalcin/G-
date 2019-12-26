# G++
A C-based Interpreted Language that I am writing for fun...

Current features:
  - Calculates integer and float point expressions
  - Stores variables

Features to be implemented:
  - Control Statements
  - Functions
  - Classes
  - Error Messages
  - Printing to the console

# Internals
The G++ program operates under 3 main modules. 
Each file is read line by line, and each passes through all modules.

Lexer:
  - The lexer is the first part of the process. It tokenizes every line, and makes sure there is nothing on the line that the language doesn't recognize. 
  - Tokenizing is the process of classifying expected words, such as keywords, identifiers, and attributes.
  -	Next, an array of tokens are passed to the parser.

Parser:
  - The parser creates a tree of operations for the interpreter to read while making sure the grammar is correct.
  - The tree contains the tokens with the same level of abstraction as the input array.
  - The tree also contains information for the precedence of operations, with the most precedent operations being on the top of the tree.
  - Next, the tree is passed into the interpreter.

Interpreter:
  - The interpreter, at this point acts on the given parse tree. The outcome of this is special to each tree (as each line has a different meaning and outcome).
  - The way we interpret the outcome or command is by the root of the tree. For example, if the root is a number operation node, the outcome of the interpretation will be a number.
  - As another example, if the root is a keyword, we know we are trying to set a variable. The variable will be added to the variable lookup table.

# Grammar Rules
(As of now)

expr	:	attribute keyword identifier = expr
	:	keyword identifier = expr
	: 	identifer = expr
	:	factor ((+|-) term)*

term	:	factor ((*|/) factor)*

factor 	:	number
	:	-number
	:	(expr)
