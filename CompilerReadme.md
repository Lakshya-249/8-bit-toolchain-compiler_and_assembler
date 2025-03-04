# Custom Procedural Language

## Overview

This is a **procedural programming language** designed for an **8-bit CPU**, supporting structured programming without object-oriented features. It includes **functions, variables, loops, conditionals, and bitwise/logical operations**.

---

## Features

### ✅ **Data Types & Variables**

- **Integer type (**\*\*\***\*`int`**\*\*\*\*\*\*)\*\*
- **Variable declarations and assignments**

```c
int a = 10;
int b = 15;
```

### ✅ **Arithmetic & Logical Expressions**

- **Arithmetic Operators:** `+`, `-`, `*`, `/`, `%`
- **Bitwise Operators:** `&`, `|`, `^`, `<<`, `>>`
- **Logical Operators:** `and`, `or`, `not` (equivalent to `&&`, `||`, `!` in C++)
- **Comparison Operators:** `==`, `!=`, `<`, `>`, `<=`, `>=`

```c
int sum = 5 + 3;
int difference = 10 - 4;
int product = 6 * 7;
int quotient = 20 / 4;
int remainder = 10 % 3;

print sum;
print difference;
print product;
print quotient;
print remainder;

int ans = 5 & 3 | 2;  // Bitwise operations
print ans;
print 0 or 65 or 3;  // Logical operations
```

### ✅ **Functions (**\*\*\***\*`def`**\*\*\*\*\*\*)\*\*

- Functions can take parameters and return values.
- Functions **support pass-by-value** and **pass-by-reference (**\*\*\***\*`ref`**\*\*\*\*\*\*)\*\*.

```c
def reverse(ref a, ref b) {
    int temp = a;
    a = b;
    b = temp;
    print a;
    print b;
}

def add(a, b) {
    int temp = a * b + b;
    return temp;
}

int x = add(10, 5);
print x;
```

### ✅ **Control Flow**

#### **If-Else Conditionals**

```c
if (a > b) {
    print "A is greater";
} else if (a == b) {
    print "A and B are equal";
} else {
    print "B is greater";
}
```

#### **Loops (**\*\*\***\*`for`**\*\*\*\***, **\*\*\*\***`while`**\*\*\*\*\*\*)\*\*

```c
for (int i = 0; i < 10; i = i + 1) {
    print i;
}

int count = 5;
while (count > 0) {
    print count;
    count = count - 1;
}
```

### ✅ **Code Blocks & Scoping**

- \*\*Scoped blocks \*\***`{}`** allow variable isolation.

```c
{
    int temp = 100;
    print temp;  // Accessible here
}
print temp;  // Error: temp is not defined outside the block
```

### ✅ **Error Handling**

- The interpreter provides **clear error messages** for:
  - **Syntax errors** (unexpected tokens, missing brackets, etc.)
  - **Runtime errors** (undefined variables, division by zero, invalid function calls)
  - **Lexical scoping errors** (accessing variables outside their scope)

---

## Credits

This project is inspired by the **8-bit computer in Verilog** created by [Lightcode](https://github.com/lightcode/8bit-computer).

---

## Example Generated Assembly Code

### **Example 1**

```assembly
.text
ldi A 10
mov M A %a
ldi A 15
mov M A %b
mov A M %a
out 0
mov A M %b
out 0
mov A M %reverse
mov A M %a
mov M A %a_local1
mov A M %b
mov M A %b_local1
call %reverse
mov A M %add
mov A M %a
mov M A %a_local1
mov A M %b
mov M A %b_local1
call %add
mov M A %x_local2
mov A M %x_local2
out 0
ldi A 2
push A
ldi A 3
push A
ldi A 5
pop B
and
pop B
or
mov M A %ans
mov A M %ans
out 0
ldi A 0
ldi B 1
cmp
jc %orand1
jmp %orandEnd1
orand1:
ldi A 65
orandEnd1:
ldi B 1
cmp
jc %orand0
jmp %orandEnd0
orand0:
ldi A 3
orandEnd0:
out 0
hlt
```

### **Example 2**

```assembly
.text
ldi A 5
mov M A %x
ldi A 19
mov M A %y
ldi A 10
mov M A %x_local1
mov A M %y
push A
mov A M %x_local1
pop B
add
out 0
```

---

## Installation & Usage

1. Clone the repository:
   ```sh
   git clone https://github.com/Lakshya-249/8-bit-toolchain-compiler_and_assembler.git
   cd Compiler
   ```
2. Build the compiler/interpreter:
   ```sh
   make
   ```
3. Run a program:
   ```sh
   ./custom_lang program.txt
   ```

---
