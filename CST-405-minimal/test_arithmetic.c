// Test file for arithmetic operations
// This demonstrates all four basic operations: +, -, *, /

int a;
int b;
int c;
int result;

// Basic arithmetic
a = 10;
b = 5;

// Addition
result = a + b;
print(result);  // Should print 15

// Subtraction
result = a - b;
print(result);  // Should print 5

// Multiplication
result = a * b;
print(result);  // Should print 50

// Division
result = a / b;
print(result);  // Should print 2

// Complex expression with precedence
// According to precedence: * and / before + and -
c = 2;
result = a + b * c;  // 10 + (5 * 2) = 10 + 10 = 20
print(result);

result = a * b + c;  // (10 * 5) + 2 = 50 + 2 = 52
print(result);

result = a - b / c;  // 10 - (5 / 2) = 10 - 2 = 8
print(result);

// Multiple operations
result = a * b - c * 3;  // (10 * 5) - (2 * 3) = 50 - 6 = 44
print(result);
