// Test file for double support
double pi;
double radius;
double area;
int x;
int y;

// Test double literals
pi = 3.14159;
radius = 5.0;

// Test double arithmetic
area = pi * radius * radius;
print(area);

// Test mixed int/double operations
x = 10;
area = x * pi;
print(area);

// Test int to double conversion
double temp;
temp = x;
print(temp);

// Test double to int conversion
y = area;
print(y);

// Test double arithmetic operations
double a;
double b;
double c;

a = 10.5;
b = 3.2;

c = a + b;
print(c);

c = a - b;
print(c);

c = a * b;
print(c);

c = a / b;
print(c);
