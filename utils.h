#pragma once
#include "bigint.h"
#include <string>

class Utils {
public:
    static BigInt gcd(BigInt a, BigInt b) {
        a = a.abs();
        b = b.abs();
        
        while (!b.isZero()) {
            BigInt temp = b;
            b = mod(a, b);
            a = temp;
        }
        return a;
    }
    
    static BigInt mod(const BigInt& a, const BigInt& b) {
        return a % b;
    }
    
    static BigInt convertFromBase(const std::string& value, int base) {
        BigInt result(0);
        BigInt baseBI(base);
        BigInt multiplier(1);
        
        for (int i = value.length() - 1; i >= 0; i--) {
            char c = value[i];
            int digit;
            
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'a' && c <= 'z') {
                digit = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'Z') {
                digit = c - 'A' + 10;
            } else {
                continue; // Skip invalid characters
            }
            
            if (digit >= base) continue; // Skip invalid digits for this base
            
            result = result + multiplier * BigInt(digit);
            multiplier = multiplier * baseBI;
        }
        
        return result;
    }
};