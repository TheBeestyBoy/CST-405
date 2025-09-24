// Simple test for mixed int/double operations
int x;
double pi;
double result;

x = 10;
pi = 3.14159;

// Test int * double
result = x * pi;
print(result);  // Should print 31.4159

// Test double * int  
result = pi * x;
print(result);  // Should print 31.4159

// Pure double test for comparison
double a;
double b;
a = 10.0;
b = 3.14159;
result = a * b;
print(result);  // Should print 31.4159
