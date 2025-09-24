.data

.text
.globl main
main:
    # Allocate stack space
    addi $sp, $sp, -400

    # Declared double pi at offset 0
    # Declared double radius at offset 8
    # Declared double area at offset 16
    # Declared int x at offset 24
    # Declared int y at offset 28
    .data
double_0: .double 3.141590
    .text
    la $t0, double_0
    ldc1 $f0, 0($t0)
    sdc1 $f0, 0($sp)
    .data
double_1: .double 5.000000
    .text
    la $t0, double_1
    ldc1 $f0, 0($t0)
    sdc1 $f0, 8($sp)
    ldc1 $f0, 0($sp)
    ldc1 $f2, 8($sp)
    mul.d $f0, $f0, $f2
    ldc1 $f2, 8($sp)
    mul.d $f0, $f0, $f2
    sdc1 $f0, 16($sp)
    ldc1 $f0, 16($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    li $t0, 10
    sw $t0, 24($sp)
    lw $t0, 24($sp)
    ldc1 $f0, 0($sp)
    mtc1 $t0, $f0
    cvt.d.w $f0, $f0
    mul.d $f0, $f0, $f0
    sdc1 $f0, 16($sp)
    ldc1 $f0, 16($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    # Declared double temp at offset 32
    lw $t0, 24($sp)
    mtc1 $t0, $f0
    cvt.d.w $f0, $f0
    sdc1 $f0, 32($sp)
    ldc1 $f0, 32($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    ldc1 $f0, 16($sp)
    cvt.w.d $f0, $f0
    mfc1 $t0, $f0
    sw $t0, 28($sp)
    lw $t0, 28($sp)
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    # Declared double a at offset 40
    # Declared double b at offset 48
    # Declared double c at offset 56
    .data
double_2: .double 10.500000
    .text
    la $t0, double_2
    ldc1 $f0, 0($t0)
    sdc1 $f0, 40($sp)
    .data
double_3: .double 3.200000
    .text
    la $t0, double_3
    ldc1 $f0, 0($t0)
    sdc1 $f0, 48($sp)
    ldc1 $f0, 40($sp)
    ldc1 $f2, 48($sp)
    add.d $f0, $f0, $f2
    sdc1 $f0, 56($sp)
    ldc1 $f0, 56($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    ldc1 $f0, 40($sp)
    ldc1 $f2, 48($sp)
    sub.d $f0, $f0, $f2
    sdc1 $f0, 56($sp)
    ldc1 $f0, 56($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    ldc1 $f0, 40($sp)
    ldc1 $f2, 48($sp)
    mul.d $f0, $f0, $f2
    sdc1 $f0, 56($sp)
    ldc1 $f0, 56($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    ldc1 $f0, 40($sp)
    ldc1 $f2, 48($sp)
    div.d $f0, $f0, $f2
    sdc1 $f0, 56($sp)
    ldc1 $f0, 56($sp)
    # Print double
    mov.d $f12, $f0
    li $v0, 3
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall

    # Exit program
    addi $sp, $sp, 400
    li $v0, 10
    syscall
