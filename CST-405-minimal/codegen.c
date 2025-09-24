#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"
#include "symtab.h"

FILE* output;
int tempReg = 0;
int tempFloatReg = 0;

int getNextTemp() {
    int reg = tempReg++;
    if (tempReg > 7) tempReg = 0;  // Reuse $t0-$t7
    return reg;
}

int getNextFloatTemp() {
    int reg = tempFloatReg;
    tempFloatReg += 2;  // Doubles use even-odd register pairs
    if (tempFloatReg > 30) tempFloatReg = 0;  // Reuse $f0-$f30
    return reg;
}

/* Check if expression produces a double result */
int isDoubleExpr(ASTNode* node) {
    if (!node) return 0;
    
    switch(node->type) {
        case NODE_DOUBLE:
            return 1;
        case NODE_VAR:
            return getVarType(node->data.name) == TYPE_DOUBLE;
        case NODE_BINOP:
            // If either operand is double, result is double
            return isDoubleExpr(node->data.binop.left) || 
                   isDoubleExpr(node->data.binop.right);
        default:
            return 0;
    }
}

void genExpr(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_NUM:
            fprintf(output, "    li $t%d, %d\n", getNextTemp(), node->data.num);
            break;
            
        case NODE_DOUBLE: {
            // Load double constant - need to use .data section
            static int doubleConstCount = 0;
            fprintf(output, "    .data\n");
            fprintf(output, "double_%d: .double %.6f\n", doubleConstCount, node->data.dbl);
            fprintf(output, "    .text\n");
            fprintf(output, "    la $t0, double_%d\n", doubleConstCount);
            fprintf(output, "    ldc1 $f%d, 0($t0)\n", getNextFloatTemp());
            doubleConstCount++;
            break;
        }
            
        case NODE_VAR: {
            int offset = getVarOffset(node->data.name);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.name);
                exit(1);
            }
            DataType varType = getVarType(node->data.name);
            if (varType == TYPE_DOUBLE) {
                fprintf(output, "    ldc1 $f%d, %d($sp)\n", getNextFloatTemp(), offset);
            } else {
                fprintf(output, "    lw $t%d, %d($sp)\n", getNextTemp(), offset);
            }
            break;
        }
        
        case NODE_BINOP: {
            int isLeftDouble = isDoubleExpr(node->data.binop.left);
            int isRightDouble = isDoubleExpr(node->data.binop.right);
            int isResultDouble = isLeftDouble || isRightDouble;
            
            // Reset registers for cleaner tracking
            int startIntReg = tempReg;
            int startFloatReg = tempFloatReg;
            
            // Generate left operand
            genExpr(node->data.binop.left);
            
            // Figure out where left operand is stored
            int leftIntReg = -1;
            int leftFloatReg = -1;
            if (isLeftDouble) {
                leftFloatReg = tempFloatReg - 2;
            } else {
                leftIntReg = tempReg - 1;
            }
            
            // Save registers if needed
            int savedLeftIntReg = leftIntReg;
            int savedLeftFloatReg = leftFloatReg;
            
            // Generate right operand
            genExpr(node->data.binop.right);
            
            // Figure out where right operand is stored
            int rightIntReg = -1;
            int rightFloatReg = -1;
            if (isRightDouble) {
                rightFloatReg = tempFloatReg - 2;
            } else {
                rightIntReg = tempReg - 1;
            }
            
            if (isResultDouble) {
                // We need both operands in floating-point registers
                int resultFloatReg = startFloatReg;
                
                // Convert left operand to double if needed
                if (!isLeftDouble) {
                    fprintf(output, "    mtc1 $t%d, $f%d\n", savedLeftIntReg, resultFloatReg);
                    fprintf(output, "    cvt.d.w $f%d, $f%d\n", resultFloatReg, resultFloatReg);
                } else {
                    resultFloatReg = savedLeftFloatReg;
                }
                
                // Convert right operand to double if needed
                int rightFloatRegToUse;
                if (!isRightDouble) {
                    rightFloatRegToUse = resultFloatReg + 2;
                    fprintf(output, "    mtc1 $t%d, $f%d\n", rightIntReg, rightFloatRegToUse);
                    fprintf(output, "    cvt.d.w $f%d, $f%d\n", rightFloatRegToUse, rightFloatRegToUse);
                } else {
                    rightFloatRegToUse = rightFloatReg;
                }
                
                // Perform floating-point operation
                switch(node->data.binop.op) {
                    case '+':
                        fprintf(output, "    add.d $f%d, $f%d, $f%d\n", resultFloatReg, resultFloatReg, rightFloatRegToUse);
                        break;
                    case '-':
                        fprintf(output, "    sub.d $f%d, $f%d, $f%d\n", resultFloatReg, resultFloatReg, rightFloatRegToUse);
                        break;
                    case '*':
                        fprintf(output, "    mul.d $f%d, $f%d, $f%d\n", resultFloatReg, resultFloatReg, rightFloatRegToUse);
                        break;
                    case '/':
                        fprintf(output, "    div.d $f%d, $f%d, $f%d\n", resultFloatReg, resultFloatReg, rightFloatRegToUse);
                        break;
                }
                tempFloatReg = resultFloatReg + 2;
                tempReg = startIntReg;  // Reset int register counter
            } else {
                // Integer operations
                switch(node->data.binop.op) {
                    case '+':
                        fprintf(output, "    add $t%d, $t%d, $t%d\n", savedLeftIntReg, savedLeftIntReg, rightIntReg);
                        break;
                    case '-':
                        fprintf(output, "    sub $t%d, $t%d, $t%d\n", savedLeftIntReg, savedLeftIntReg, rightIntReg);
                        break;
                    case '*':
                        fprintf(output, "    mul $t%d, $t%d, $t%d\n", savedLeftIntReg, savedLeftIntReg, rightIntReg);
                        break;
                    case '/':
                        fprintf(output, "    div $t%d, $t%d\n", savedLeftIntReg, rightIntReg);
                        fprintf(output, "    mflo $t%d\n", savedLeftIntReg);  // Get quotient from LO register
                        break;
                    default:
                        fprintf(stderr, "Error: Unknown operator %c\n", node->data.binop.op);
                        exit(1);
                }
                tempReg = savedLeftIntReg + 1;
                tempFloatReg = startFloatReg;  // Reset float register counter
            }
            break;
        }
            
        default:
            break;
    }
}

void genStmt(ASTNode* node) {
    if (!node) return;
    
    switch(node->type) {
        case NODE_DECL: {
            int offset = addVar(node->data.decl.name, node->data.decl.type);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s already declared\n", node->data.decl.name);
                exit(1);
            }
            fprintf(output, "    # Declared %s %s at offset %d\n", 
                    node->data.decl.type == TYPE_INT ? "int" : "double",
                    node->data.decl.name, offset);
            break;
        }
        
        case NODE_ASSIGN: {
            int offset = getVarOffset(node->data.assign.var);
            if (offset == -1) {
                fprintf(stderr, "Error: Variable %s not declared\n", node->data.assign.var);
                exit(1);
            }
            DataType varType = getVarType(node->data.assign.var);
            genExpr(node->data.assign.value);
            
            if (varType == TYPE_DOUBLE) {
                if (isDoubleExpr(node->data.assign.value)) {
                    fprintf(output, "    sdc1 $f%d, %d($sp)\n", tempFloatReg - 2, offset);
                } else {
                    // Convert int to double before storing
                    fprintf(output, "    mtc1 $t%d, $f0\n", tempReg - 1);
                    fprintf(output, "    cvt.d.w $f0, $f0\n");
                    fprintf(output, "    sdc1 $f0, %d($sp)\n", offset);
                }
                tempFloatReg = 0;
            } else {
                if (isDoubleExpr(node->data.assign.value)) {
                    // Convert double to int before storing
                    fprintf(output, "    cvt.w.d $f%d, $f%d\n", tempFloatReg - 2, tempFloatReg - 2);
                    fprintf(output, "    mfc1 $t0, $f%d\n", tempFloatReg - 2);
                    fprintf(output, "    sw $t0, %d($sp)\n", offset);
                } else {
                    fprintf(output, "    sw $t%d, %d($sp)\n", tempReg - 1, offset);
                }
            }
            tempReg = 0;
            tempFloatReg = 0;
            break;
        }
        
        case NODE_PRINT:
            genExpr(node->data.expr);
            if (isDoubleExpr(node->data.expr)) {
                fprintf(output, "    # Print double\n");
                fprintf(output, "    mov.d $f12, $f%d\n", tempFloatReg - 2);
                fprintf(output, "    li $v0, 3\n");  // Print double syscall
                fprintf(output, "    syscall\n");
                tempFloatReg = 0;
            } else {
                fprintf(output, "    # Print integer\n");
                fprintf(output, "    move $a0, $t%d\n", tempReg - 1);
                fprintf(output, "    li $v0, 1\n");  // Print int syscall
                fprintf(output, "    syscall\n");
                tempReg = 0;
            }
            fprintf(output, "    # Print newline\n");
            fprintf(output, "    li $v0, 11\n");
            fprintf(output, "    li $a0, 10\n");
            fprintf(output, "    syscall\n");
            break;
            
        case NODE_STMT_LIST:
            genStmt(node->data.stmtlist.stmt);
            genStmt(node->data.stmtlist.next);
            break;
            
        default:
            break;
    }
}

void generateMIPS(ASTNode* root, const char* filename) {
    output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Cannot open output file %s\n", filename);
        exit(1);
    }
    
    // Initialize symbol table
    initSymTab();
    
    // MIPS program header
    fprintf(output, ".data\n");
    fprintf(output, "\n.text\n");
    fprintf(output, ".globl main\n");
    fprintf(output, "main:\n");
    
    // Allocate stack space (max 100 variables * 4 bytes)
    fprintf(output, "    # Allocate stack space\n");
    fprintf(output, "    addi $sp, $sp, -400\n\n");
    
    // Generate code for statements
    genStmt(root);
    
    // Program exit
    fprintf(output, "\n    # Exit program\n");
    fprintf(output, "    addi $sp, $sp, 400\n");
    fprintf(output, "    li $v0, 10\n");
    fprintf(output, "    syscall\n");
    
    fclose(output);
}
