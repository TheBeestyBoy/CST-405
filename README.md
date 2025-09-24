# C-Minus Compiler
## CST-405 Compiler Design Course

A complete compiler implementation for the C-Minus language, demonstrating all phases of compilation from source code to MIPS assembly.

## Overview

This project implements a full compiler for C-Minus, a simplified subset of C, using traditional compiler construction tools (Flex and Bison) along with hand-written components for semantic analysis, code generation, and optimization.

### Compilation Phases

1. **Lexical Analysis** (Flex) - Tokenizes the source code
2. **Syntax Analysis** (Bison) - Parses tokens and builds parse tree
3. **AST Construction** - Builds abstract syntax tree
4. **Semantic Analysis** - Type checking and symbol table management
5. **Intermediate Code Generation** - Generates three-address code
6. **Optimization** - Performs various code optimizations
7. **Target Code Generation** - Produces MIPS assembly code

## Project Structure

```
cminus-compiler/
├── src/                 # Source files
│   ├── lexer.l         # Flex lexer specification
│   ├── parser.y        # Bison parser specification
│   ├── ast.c           # AST implementation
│   ├── symtab.c        # Symbol table
│   ├── semantic.c      # Semantic analyzer
│   ├── codegen.c       # 3-address code generator
│   ├── optimize.c      # Optimizer
│   ├── mips.c          # MIPS code generator
│   ├── main.c          # Main driver
│   └── util.c          # Utility functions
├── include/            # Header files
│   ├── globals.h       # Global definitions
│   ├── ast.h           # AST declarations
│   ├── symtab.h        # Symbol table declarations
│   ├── semantic.h      # Semantic analyzer declarations
│   ├── codegen.h       # Code generation declarations
│   ├── optimize.h      # Optimizer declarations
│   ├── mips.h          # MIPS generator declarations
│   └── util.h          # Utility declarations
├── tests/              # Sample C-Minus programs
│   ├── simple.cm       # Basic arithmetic
│   ├── factorial.cm    # Recursive factorial
│   ├── fibonacci.cm    # Fibonacci sequence
│   ├── gcd.cm          # Greatest common divisor
│   └── sort.cm         # Bubble sort
├── docs/               # Documentation
│   └── grammar.txt     # C-Minus grammar specification
├── Makefile            # Build configuration
└── README.md           # This file
```

## Building the Compiler

### Prerequisites

- GCC compiler
- Flex (lexical analyzer generator)
- Bison (parser generator)
- Make build tool

### Build Instructions

```bash
# Build the compiler
make

# Clean build artifacts
make clean

# Run tests
make test
```

## Using the Compiler

### Basic Usage

```bash
# Compile a C-Minus program
./cminus source.cm

# This generates source.s (MIPS assembly)
```

### Command Line Options

```bash
./cminus [options] source_file.cm

Options:
  -h, --help         Show help message
  -s, --trace-scan   Enable scanner tracing
  -p, --trace-parse  Enable parser tracing and show AST
  -a, --trace-sem    Enable semantic analysis tracing
  -c, --trace-code   Enable code generation tracing
  -O<level>          Set optimization level (0-2)
  -n, --no-code      Disable code generation
  -o <file>          Specify output file

Examples:
  ./cminus -p test.cm        # Show AST
  ./cminus -O2 test.cm       # Optimize level 2
  ./cminus -spac test.cm     # Enable all tracing
```

## C-Minus Language Features

### Data Types
- `int` - Integer type
- `void` - Void type (for functions)
- Arrays of integers

### Statements
- Variable declarations
- Function declarations
- Assignment statements
- If-else statements
- While loops
- Return statements
- Compound statements (blocks)

### Expressions
- Arithmetic: `+`, `-`, `*`, `/`
- Relational: `<`, `<=`, `>`, `>=`, `==`, `!=`
- Function calls
- Array indexing

### Built-in Functions
- `input()` - Read integer from user
- `output(int)` - Print integer to console

## Example Programs

### Factorial (Recursive)
```c
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

void main(void) {
    int num;
    num = input();
    output(factorial(num));
}
```

### Fibonacci Sequence
```c
void main(void) {
    int n;
    int fib[20];
    int i;
    
    n = input();
    fib[0] = 0;
    fib[1] = 1;
    
    i = 2;
    while (i < n) {
        fib[i] = fib[i-1] + fib[i-2];
        i = i + 1;
    }
    
    i = 0;
    while (i < n) {
        output(fib[i]);
        i = i + 1;
    }
}
```

## Compiler Internals

### Abstract Syntax Tree (AST)

The compiler builds an AST from the parse tree, removing syntactic details and focusing on semantic structure. This makes subsequent analysis and code generation more efficient.

**Parse Tree vs AST Example:**
```
Expression: x = 2 + 3 * 4

Parse Tree (detailed):              AST (simplified):
     assignment                          ASSIGN
     /    |    \                         /    \
   var   '='  expr                    ID:x     +
    |           |                             /   \
   ID:x    add-expr                        NUM:2   *
           /   |   \                              /   \
        term  '+'  term                       NUM:3  NUM:4
         |          |
       NUM:2   term * factor
                |       |
              NUM:3   NUM:4
```

### Symbol Table

Hierarchical scope-based symbol table with support for:
- Global and local scopes
- Function parameters
- Array declarations
- Type checking
- Memory allocation tracking

### Three-Address Code (TAC) Generation

**Overview:**
Three-Address Code is an intermediate representation where each instruction has at most three operands (result = arg1 op arg2). This simplifies both optimization and final code generation.

**TAC Generation Process:**
1. **AST to TAC Conversion**: Each AST node type maps to specific TAC instructions
   - Declarations: `NODE_DECL` → `TAC_DECL`
   - Assignments: `NODE_ASSIGN` → `TAC_ASSIGN`
   - Expressions: `NODE_BINOP` → `TAC_ADD` with temporary variables
   - Print statements: `NODE_PRINT` → `TAC_PRINT`

2. **Expression Handling**: Complex expressions use temporary variables
   - Numbers: Converted to string literals
   - Variables: Referenced by name
   - Binary operations: Create temporary variables (t0, t1, t2...)

**Example TAC Generation:**
```c
// Source Code:
int x; int y;
x = 10; y = 20;
print(x + y);

// Generated TAC:
1: DECL x          // Declare variable 'x'
2: DECL y          // Declare variable 'y'
3: x = 10          // Assign constant to x
4: y = 20          // Assign constant to y
5: t0 = x + y      // Add: store result in temp t0
6: PRINT t0        // Output value of t0
```

### TAC Optimization Implementation

**Optimization Techniques:**
1. **Constant Folding** - Evaluates compile-time constant expressions
   - `10 + 20` becomes `30` directly
2. **Copy Propagation** - Replaces variable references with their known values
   - Uses a value propagation table to track variable-value mappings

**Optimization Process:**
1. **Track assignments**: Store variable-value mappings in propagation table
2. **Substitute references**: Replace variables with known values
3. **Fold constants**: Evaluate constant expressions at compile-time
4. **Propagate results**: Update the propagation table with new constants

**Optimization Example:**
```c
// Original TAC:
1: DECL x
2: DECL y
3: x = 10          // x now maps to "10"
4: y = 20          // y now maps to "20"
5: t0 = x + y      // Substitute: t0 = 10 + 20
6: PRINT t0

// Optimized TAC:
1: DECL x
2: DECL y
3: x = 10          // Constant value: 10
4: y = 20          // Constant value: 20
5: t0 = 30         // Folded: 10 + 20 = 30
6: PRINT 30        // Propagated constant
```

**Additional Optimization Techniques (for advanced implementations):**
1. **Dead Code Elimination** - Remove unreachable or unused code
2. **Algebraic Simplification** - Simplify expressions (x+0 → x, x*1 → x)
3. **Common Subexpression Elimination** - Reuse computed values

### MIPS Code Generation Implementation

**Overview:**
The final phase converts the AST directly to MIPS assembly, using the symbol table for variable memory management and register allocation for temporary values.

**MIPS Architecture Used:**
- **Registers:**
  - `$sp`: Stack pointer (points to top of stack)
  - `$t0-$t7`: Temporary registers for computations
  - `$a0`: Argument register for system calls
  - `$v0`: System call number register
- **Memory Layout:**
  - Variables stored on stack with negative offsets from `$sp`
  - Stack grows downward (decreasing addresses)
  - Each integer variable occupies 4 bytes

**Code Generation Process:**

**1. Program Initialization:**
```mips
.data                    # Data section (empty for simple language)
.text                    # Code section
.globl main             # Make main globally visible
main:                   # Program entry point
    addi $sp, $sp, -400 # Allocate 400 bytes stack space
```

**2. Variable Operations:**
- **Declaration**: Adds to symbol table, generates comment
- **Assignment**: `li $t0, 10; sw $t0, 0($sp)` (load immediate, store word)
- **Variable Access**: `lw $t1, 0($sp)` (load word from stack offset)
- **Addition**: `lw $t0, 0($sp); lw $t1, 4($sp); add $t0, $t0, $t1`

**3. Print Statement:**
```mips
move $a0, $t0     # Move value to argument register
li $v0, 1         # System call number for print integer
syscall           # Execute system call
li $v0, 11        # System call for print character
li $a0, 10        # ASCII newline character
syscall           # Print newline
```

**Complete MIPS Example:**
```c
// Source: int x; int y; x = 10; y = 20; print(x + y);
```
```mips
.data
.text
.globl main
main:
    addi $sp, $sp, -400    # Allocate stack

    # x = 10;
    li $t0, 10
    sw $t0, 0($sp)

    # y = 20;
    li $t1, 20
    sw $t1, 4($sp)

    # print(x + y);
    lw $t0, 0($sp)         # Load x
    lw $t1, 4($sp)         # Load y
    add $t0, $t0, $t1      # x + y
    move $a0, $t0          # Prepare for print
    li $v0, 1              # Print integer
    syscall

    # Exit program
    addi $sp, $sp, 400
    li $v0, 10
    syscall
```

**Symbol Table Integration:**
The MIPS generator uses the symbol table for:
- Variable declarations: Add to symbol table, get stack offset
- Variable access: Look up stack offset for load/store operations
- Error detection: Verify variables are declared before use

**Advanced Features (for full compiler):**
- Register allocation (using $t0-$t7, $s0-$s7)
- Stack frame management
- Function calling conventions
- System calls for I/O

## Educational Value

This compiler demonstrates:

1. **Lexical Analysis** - Pattern matching with regular expressions
2. **Parsing** - Context-free grammars and LALR parsing
3. **AST Construction** - Tree data structures and traversal
4. **Symbol Tables** - Hash tables and scope management
5. **Type Checking** - Static semantic analysis
6. **Code Generation** - Instruction selection and register allocation
7. **Optimization** - Data flow analysis and transformation
8. **Target Architecture** - MIPS assembly and calling conventions

## Testing

Run the test suite:
```bash
make test
```

Individual test programs:
```bash
./cminus tests/factorial.cm
./cminus tests/fibonacci.cm
./cminus tests/sort.cm
```

## Limitations

- No floating-point support
- No string support
- No dynamic memory allocation
- Limited to 32-bit integers
- No preprocessor directives
- No separate compilation/linking

## Future Enhancements

- Add support for more data types (char, float)
- Implement more optimization passes
- Add register allocation with graph coloring
- Support for multiple source files
- Generate x86 or ARM assembly
- Add debugging information
- Implement error recovery

## References

- Aho, Sethi, Ullman - "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- Flex & Bison documentation
- MIPS assembly language reference
- C-Minus language specification

## Author

Created for CST-405 Compiler Design Course

## License

Educational use only