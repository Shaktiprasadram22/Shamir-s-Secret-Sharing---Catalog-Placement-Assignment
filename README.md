# Shamir's Secret Sharing - Catalog Placement Assignment

🎯 **Overview**

This project implements a simplified version of Shamir's Secret Sharing algorithm to reconstruct the constant term of a polynomial using Lagrange interpolation. The solution handles large 256-bit integers and various base encodings without using external libraries.

## 📁 Project Structure

```
catalog_solver/
├── main.cpp              # Entry point - processes both test cases
├── bigint.h              # Custom BigInt class for 256-bit arithmetic
├── fraction.h            # Rational number class for exact arithmetic
├── interpolation.h       # Lagrange interpolation implementation
├── parser.h              # JSON parser and base conversion
├── utils.h               # Utility functions (GCD, base conversion)
├── input.json            # Test case data
└── README.md             # This file
```

## 🧮 Mathematical Logic

### Problem Statement

- **Given**: Unknown polynomial f(x) = a_m x^m + ... + a_1 x + c
- **Goal**: Find the constant term c (which equals f(0))
- **Input**: k points (x_i, y_i) where y_i = f(x_i)
- **Constraint**: Need minimum k = m + 1 points to solve

### Solution Approach

We use Lagrange Interpolation to directly compute f(0):

```
f(0) = Σ(i=0 to k-1) y_i × L_i(0)
```

Where L_i(0) is the Lagrange basis polynomial:

```
L_i(0) = Π(j≠i) (-x_j)/(x_i - x_j)
```

## 🔧 Key Components

### 1. BigInt Class (bigint.h)

- **Purpose**: Handle 256-bit integers without external libraries
- **Key Operations**: Addition, subtraction, multiplication, division, modulo
- **Storage**: Vector of digits in reverse order for efficient arithmetic
- **Challenge**: Implementing efficient long division algorithm

### 2. Fraction Class (fraction.h)

- **Purpose**: Exact rational arithmetic to avoid floating-point errors
- **Features**: Automatic simplification using GCD
- **Critical**: Ensures precise calculations during interpolation

### 3. Base Conversion (utils.h)

- **Challenge**: Convert values from bases 2-16 to decimal
- **Implementation**:
  ```cpp
  result = result + digit × base^position
  ```
- **Handles**: Both numeric (0-9) and alphabetic (a-f) characters

### 4. Lagrange Interpolation (interpolation.h)

- **Core Algorithm**:
  ```cpp
  for each point (x_i, y_i):
      term = y_i
      for each other point (x_j, y_j):
          term *= (-x_j) / (x_i - x_j)
      result += term
  ```

## 🎲 Tricky Parts & Solutions

### 1. Large Number Arithmetic

- **Challenge**: Handle 256-bit integers efficiently
- **Solution**:
  - Custom BigInt with vector<int> storage
  - Implemented proper long division (not repeated subtraction)
  - Optimized multiplication using grade-school algorithm

### 2. Precision in Rational Arithmetic

- **Challenge**: Avoid floating-point precision errors
- **Solution**:
  - Used Fraction class with BigInt numerator/denominator
  - All calculations done in exact rational arithmetic
  - Final result converted to integer only at the end

### 3. Base Conversion with Large Numbers

- **Challenge**: Convert very long strings from various bases
- **Solution**:
  - Process digits from right to left
  - Use BigInt multiplication for powers of base
  - Handle both numeric and hexadecimal characters

### 4. JSON Parsing Without Libraries

- **Challenge**: Parse JSON structure manually
- **Solution**:
  - Simple string searching for key patterns
  - Extract numeric values and string values separately
  - Handle nested structure for points data

## 🚀 How to Run

### Prerequisites

- C++ compiler with C++11 support (g++ recommended)
- No external libraries required

### Compilation

```bash
# Navigate to project directory
cd catalog_solver

# Compile the project
g++ -std=c++11 -O2 main.cpp -o catalog_solver

# Or with debug symbols
g++ -std=c++11 -g -Wall main.cpp -o catalog_solver
```

### Execution

```bash
# Run the solver
./catalog_solver

# Expected output:
# Processing Test Case 1...
# Secret for Test Case 1: 3
#
# Processing Test Case 2...
# Secret for Test Case 2: [large number]
```

### Input Format

The input.json file should contain:

```json
{
  "keys": {
    "n": 4,
    "k": 3
  },
  "1": {
    "base": "10",
    "value": "4"
  },
  "2": {
    "base": "2",
    "value": "111"
  },
  "3": {
    "base": "10",
    "value": "12"
  },
  "6": {
    "base": "4",
    "value": "213"
  }
}
```

## 📊 Test Cases

### Test Case 1

- **Points**: (1,4), (2,7), (3,12), (6,39)
- **Expected Secret**: 3
- **Verification**: Manual calculation confirms f(0) = 3

### Test Case 2

- **Points**: 10 points with large base-encoded values
- **Expected**: Large integer (256-bit range)
- **Challenge**: Tests BigInt arithmetic with real-world sized numbers

## 🔍 Verification

To manually verify Test Case 1:

1. Decode y-values: "111" (base 2) = 7, "213" (base 4) = 39
2. Apply Lagrange formula with points (1,4), (2,7), (3,12)
3. Calculate: f(0) = 4×3 + 7×(-3) + 12×1 = 12 - 21 + 12 = 3 ✓

## ⚡ Performance Notes

- **Time Complexity**: O(k²) for k points
- **Space Complexity**: O(n) for storing large integers
- **Optimization**: Only uses minimum required points (k out of n)

## 🎯 Key Learnings

- **Mathematical Insight**: Direct computation of f(0) is more efficient than finding all coefficients
- **Precision Matters**: Exact rational arithmetic prevents accumulation of errors
- **Custom Implementation**: Sometimes building from scratch gives better control than external libraries
- **Algorithm Choice**: Lagrange interpolation is more suitable than matrix methods for this specific problem

## Results

```
=== RESULTS ===
Test Case 1 Secret: 3
Test Case 2 Secret: 75370654919612532327
```
