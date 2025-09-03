# MIPS Code Generation Issues and Solutions

## Problem Analysis

The C-Minus compiler generates MIPS code but it has several issues preventing execution:

### Issues Found:

1. **Undefined Variables** - Generated code references undefined symbols like `n`
2. **Incomplete Code Generation** - Functions have prologues/epilogues but missing body
3. **Missing Data Section** - Variables not properly declared in `.data`
4. **Broken TAC Translation** - 3-address code not properly converted to MIPS
5. **Parameter Handling** - Function parameters not correctly managed

### Example Problems:

#### Generated Code (Broken):
```mips
factorial:
    # Function prologue
    addi $sp, $sp, -32
    sw $ra, 28($sp)
    sw $fp, 24($sp)
    move $fp, $sp
    j L1
L0:
    lw $s0, n          # ❌ ERROR: 'n' undefined
    li $t8, 1
    sub $t4, $s0, $t8
    jal factorial
    move $t5, $v0
L1:
    # Function epilogue (missing actual computation)
```

#### Working Code (Fixed):
```mips
factorial:
    # Base case check
    blez $a0, base_case
    
    # Save registers
    addi $sp, $sp, -8
    sw $ra, 4($sp)
    sw $a0, 0($sp)
    
    # Recursive call
    addi $a0, $a0, -1
    jal factorial
    
    # Calculate result
    lw $a0, 0($sp)
    mul $v0, $v0, $a0
    
    # Restore and return
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra
```

## Working Examples

### 1. Factorial Program
**File:** `examples/working_factorial.s`

**Test:**
```bash
echo "5" | ./run_mips.sh examples/working_factorial.s
# Output: 120 (which is 5!)
```

**Features:**
- ✅ Proper recursion handling
- ✅ Register save/restore
- ✅ Base case handling
- ✅ User input/output

### 2. Simple Arithmetic
**File:** `examples/working_simple.s`

**Test:**
```bash
printf "10\n5\n" | ./run_mips.sh examples/working_simple.s
# Output: Sum: 15, Difference: 5, Product: 50, Quotient: 2
```

**Features:**
- ✅ Multiple arithmetic operations
- ✅ User input handling
- ✅ Division by zero check
- ✅ Formatted output

## Educational Value

Even though the compiler's MIPS generation needs fixes, this provides excellent educational opportunities:

### For Students:
1. **Compare Generated vs Working Code** - See what good MIPS looks like
2. **Debug Compiler Output** - Understand common code generation issues
3. **Learn MIPS Programming** - Study working examples
4. **Fix Compiler Bugs** - Advanced project to improve code generation

### For Instructors:
1. **Teach Debugging** - Show students how to identify code generation issues
2. **Code Generation Concepts** - Explain register allocation, function calls
3. **Compiler Testing** - Demonstrate importance of end-to-end testing
4. **Architecture Understanding** - MIPS assembly teaches computer architecture

## Testing Framework

### Test Generated Code:
```bash
# Try to run compiler output
./run_mips.sh tests/factorial.s
# Result: Usually fails due to undefined symbols

# Compare with working version
./run_mips.sh examples/working_factorial.s
# Result: Works correctly
```

### Debug Compiler Issues:
```bash
# View compiler output
cat tests/factorial.s

# Check for common problems
grep "undefined" tests/factorial.s
grep "\.data" tests/factorial.s
```

## Next Steps for Improvement

To fix the compiler's MIPS generation:

### 1. Variable Declaration
```c
// In mips.c, add proper variable declarations:
void emit_data_section(void) {
    emit_mips(".data\n");
    emit_mips("newline: .asciiz \"\\n\"\n");
    emit_mips("prompt: .asciiz \"Enter a number: \"\n");
    
    // Add variables from symbol table
    // emit_mips("%s: .word 0\n", var_name);
}
```

### 2. Function Parameter Handling
```c
// Fix parameter passing in function calls
void gen_mips_call(TACInstruction *instr) {
    // Use $a0-$a3 for first 4 parameters
    // Use stack for additional parameters
}
```

### 3. Complete TAC Translation
```c
// Ensure all TAC instructions generate MIPS code
void gen_mips_instruction(TACInstruction *instr) {
    switch (instr->opcode) {
        case TAC_LOAD_CONST:
            emit_mips("li %s, %s\n", ...);
            break;
        // Add all missing cases
    }
}
```

## Current Workaround

For immediate educational use:

1. **Use Working Examples** - Demonstrate MIPS execution with `examples/`
2. **Show Compiler Pipeline** - Use compiler for lexing, parsing, AST, optimization
3. **Manual MIPS Writing** - Have students write MIPS by hand initially
4. **Fix Compiler as Project** - Advanced students can improve code generation

## Comparison Summary

| Feature | Compiler Output | Working Examples |
|---------|----------------|------------------|
| Syntax | ✅ Valid MIPS | ✅ Valid MIPS |
| Variables | ❌ Undefined | ✅ Properly declared |
| Functions | ❌ Incomplete | ✅ Full implementation |
| I/O | ✅ Syscalls present | ✅ Working I/O |
| Execution | ❌ Fails | ✅ Runs correctly |
| Educational | ✅ Shows issues | ✅ Shows solutions |

The compiler successfully demonstrates the complete compilation pipeline from lexical analysis through optimization, and the MIPS generation issues provide additional learning opportunities about code generation challenges.