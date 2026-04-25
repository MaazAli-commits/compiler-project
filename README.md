<div align="center">

# C-like Language Compiler with Bytecode VM and Runtime Profiling

</div>

---

## Overview

This project implements a compiler for a C-like programming language in C++, covering the complete compilation pipeline — from lexical analysis to bytecode execution.

The compiler translates source code into a custom bytecode instruction set, which is executed by a purpose-built stack-based virtual machine. A runtime profiling layer tracks instruction execution frequency to identify hot paths, inspired by JIT-style optimization techniques used in modern compute systems.

---

## Features

- Lexical analysis (tokenization of source code)
- Recursive descent parser for syntax analysis
- Abstract Syntax Tree (AST) construction
- Support for control flow — `if/else`, `while` loops
- Scoped symbol table with nested block-level variable resolution
- Custom stack-based bytecode instruction set
- Virtual Machine (VM) for bytecode execution
- Compile-time constant folding optimization
- Runtime profiling to identify hot execution paths
- Error handling — division by zero, use of undeclared variables

---

## Compilation Pipeline

```
Source Code → Lexer → Tokens → Parser → AST → Codegen → Bytecode → VM → Output
```

Each stage has a single responsibility:

| Stage | File | Role |
|---|---|---|
| Lexer | `lexer.cpp` | Breaks source into tokens |
| Parser | `parser.cpp` | Builds AST from tokens |
| Codegen | `codegen.cpp` | Emits bytecode from AST + constant folding |
| VM | `vm.cpp` | Executes bytecode + runtime profiling |

---

## Example

### Input (`examples/test.cl`)

```c
int x = 10;
int y = 3;
int z = x + y;
print(z);

if (x > y) {
    print(1);
} else {
    print(0);
}

int i = 0;
while (i < 5) {
    print(i);
    i = i + 1;
}
```

### Output

```
13
1
0
1
2
3
4
```

---

## Constant Folding

Constant expressions are evaluated at compile time, reducing runtime instruction count.

### Example

```c
int x = 2 + 3 * 4;
print(x);
```

Instead of emitting `PUSH 2`, `PUSH 3`, `PUSH 4`, `MUL`, `ADD` — the compiler folds this to a single `PUSH 14` at compile time.

**Output:** `14`

---

## Runtime Profiling

The VM tracks how many times each bytecode instruction executes during a run. Instructions that execute frequently are marked as **HOT**, identifying the most performance-critical paths.

### Example Output (while loop)

```
Runtime Profile
  [8] count=12  << HOT
  [9] count=12  << HOT
  [10] count=12  << HOT
```

This is conceptually similar to how JIT compilers in modern compute frameworks detect hot paths for targeted optimization.

---

## Error Handling

| Error | Behavior |
|---|---|
| Division by zero | `Error: Division by zero` |
| Variable used before declaration | `Error: Variable 'x' used before declaration` |

---

## Supported Language Features

```c
// Variable declaration
int x = 5;

// Assignment
x = x + 1;

// Arithmetic
int y = (2 + 3) * 4;

// Comparison
if (x == y) {
    print(1);
} else {
    print(0);
}

// Loops
while (x < 10) {
    x = x + 1;
}

// Print
print(x);
```

---

## How to Build and Run

### Compile

```bash
g++ -std=c++17 src/*.cpp -o compiler
```

### Run

```bash
./compiler examples/test.cl
```

---

## Project Structure

```
compiler-project/
├── src/
│   ├── lexer.h / lexer.cpp       # Tokenizer
│   ├── ast.h                     # AST node definitions
│   ├── parser.h / parser.cpp     # Recursive descent parser
│   ├── codegen.h / codegen.cpp   # Bytecode generation + constant folding
│   ├── vm.h / vm.cpp             # Virtual machine + runtime profiler
│   └── main.cpp                  # Entry point
├── examples/                     # Sample .cl programs
└── README.md
```

---

## Technologies Used

- C++ (C++17)
- Standard Template Library (STL)
- Manual memory management via `unique_ptr`

---

## Motivation

Built this to understand how compilers actually work under the hood, not just theoretically.

---

## Author

**Mohammed Maaz Ali**
B.Tech Computer Science Engineering
IIIT Kottayam
