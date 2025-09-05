# Minimal C Compiler - Educational Version

A simplified compiler demonstrating the core phases of compilation with minimal complexity.

## Language Features
- Integer variable declarations (`int x;`)
- Assignment statements (`x = 10;`)
- Addition operator (`x + y`)
- Print statement (`print(x);`)

## Compiler Phases

### 1. Lexical Analysis (scanner.l)
- Tokenizes: keywords (int, print), identifiers, numbers, operators (+, =), delimiters

### 2. Syntax Analysis (parser.y)
- Grammar rules for declarations, assignments, expressions, print statements
- Builds Abstract Syntax Tree (AST)

### 3. Semantic Analysis (symtab.c)
- Simple symbol table tracking variable names and stack offsets
- Checks for undeclared variables and redeclarations

### 4. Code Generation (codegen.c)
- Generates MIPS assembly code
- Stack-based variable storage
- Simple register allocation ($t0-$t7)

## Build & Run

```bash
make
./minicompiler test.c output.s
```

## Example Program
```c
int x;
int y;
x = 10;
y = 20;
print(x + y);  // Output: 30
```

## Generated MIPS Structure
- Variables stored on stack
- Integer arithmetic using add instruction
- Print using syscall 1 (print integer)
- Clean exit with syscall 10