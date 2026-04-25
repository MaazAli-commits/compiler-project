\# Imperative Language Compiler with Bytecode VM (C++)



\## Overview



This project implements a compiler for a simplified imperative programming language in C++.

It covers the complete compilation pipeline—from lexical analysis to execution—using a custom bytecode and virtual machine.



\## Features



\* Lexical analysis (tokenization of source code)

\* Recursive descent parser for syntax analysis

\* Abstract Syntax Tree (AST) construction

\* Custom stack-based bytecode instruction set

\* Virtual Machine (VM) for executing bytecode

\* Runtime profiling to track instruction execution frequency



\## Compilation Pipeline



Source Code → Tokens → AST → Bytecode → Virtual Machine Execution



\## Example



\### Input (`examples/test.cl`)



```

x = 2 + 3

print x

```



\### Output



```

5

```



\## How to Run



\### Compile



```

g++ src/main.cpp -o compiler

```



\### Run



```

./compiler examples/test.cl

```



\## Project Structure



```

compiler\_proj/

├── src/        # Compiler implementation (lexer, parser, AST, VM)

├── examples/   # Sample programs demonstrating features

```



\## Motivation



This project was built to gain hands-on experience with compiler design concepts such as parsing, intermediate representations, and execution models, and to explore how runtime systems execute and optimize programs.



