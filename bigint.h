#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

class BigInt {
private:
    std::vector<int> digits;
    bool negative;
    
    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }
    
public:
    BigInt() : negative(false) {
        digits.push_back(0);
    }
    
    BigInt(long long num) : negative(num < 0) {
        if (num < 0) num = -num;
        if (num == 0) {
            digits.push_back(0);
        } else {
            while (num > 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }
    
    BigInt(const std::string& str) {
        negative = false;
        int start = 0;
        if (str[0] == '-') {
            negative = true;
            start = 1;
        }
        
        for (int i = str.length() - 1; i >= start; i--) {
            digits.push_back(str[i] - '0');
        }
        removeLeadingZeros();
    }
    
    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }
    
    bool isNegative() const {
        return negative && !isZero();
    }
    
    BigInt abs() const {
        BigInt result = *this;
        result.negative = false;
        return result;
    }
    
    int compare(const BigInt& other) const {
        if (negative != other.negative) {
            return negative ? -1 : 1;
        }
        
        int sign = negative ? -1 : 1;
        
        if (digits.size() != other.digits.size()) {
            return sign * (digits.size() < other.digits.size() ? -1 : 1);
        }
        
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != other.digits[i]) {
                return sign * (digits[i] < other.digits[i] ? -1 : 1);
            }
        }
        return 0;
    }
    
    bool operator<(const BigInt& other) const {
        return compare(other) < 0;
    }
    
    bool operator>(const BigInt& other) const {
        return compare(other) > 0;
    }
    
    bool operator==(const BigInt& other) const {
        return compare(other) == 0;
    }
    
    bool operator!=(const BigInt& other) const {
        return compare(other) != 0;
    }
    
    bool operator<=(const BigInt& other) const {
        return compare(other) <= 0;
    }
    
    bool operator>=(const BigInt& other) const {
        return compare(other) >= 0;
    }
    
    BigInt operator+(const BigInt& other) const {
        if (negative != other.negative) {
            if (negative) {
                return other - abs();
            } else {
                return *this - other.abs();
            }
        }
        
        BigInt result;
        result.negative = negative;
        result.digits.clear();
        
        int carry = 0;
        size_t maxSize = std::max(digits.size(), other.digits.size());
        
        for (size_t i = 0; i < maxSize || carry; i++) {
            int sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];
            
            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        
        return result;
    }
    
    BigInt operator-(const BigInt& other) const {
        if (negative != other.negative) {
            BigInt result = abs() + other.abs();
            result.negative = negative;
            return result;
        }
        
        if (abs() < other.abs()) {
            BigInt result = other.abs() - abs();
            result.negative = !negative;
            return result;
        }
        
        BigInt result;
        result.negative = negative;
        result.digits.clear();
        
        int borrow = 0;
        for (size_t i = 0; i < digits.size(); i++) {
            int diff = digits[i] - borrow;
            if (i < other.digits.size()) diff -= other.digits[i];
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            result.digits.push_back(diff);
        }
        
        result.removeLeadingZeros();
        return result;
    }
    
    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.assign(digits.size() + other.digits.size(), 0);
        result.negative = negative != other.negative;
        
        for (size_t i = 0; i < digits.size(); i++) {
            for (size_t j = 0; j < other.digits.size(); j++) {
                result.digits[i + j] += digits[i] * other.digits[j];
                if (result.digits[i + j] >= 10) {
                    result.digits[i + j + 1] += result.digits[i + j] / 10;
                    result.digits[i + j] %= 10;
                }
            }
        }
        
        result.removeLeadingZeros();
        return result;
    }
    
    BigInt operator/(const BigInt& other) const {
        if (other.isZero()) {
            throw std::runtime_error("Division by zero");
        }
        
        BigInt dividend = abs();
        BigInt divisor = other.abs();
        
        if (dividend < divisor) return BigInt(0);
        
        BigInt quotient;
        quotient.digits.assign(dividend.digits.size(), 0);
        BigInt remainder(0);
        
        // Long division algorithm
        for (int i = dividend.digits.size() - 1; i >= 0; i--) {
            remainder = remainder * BigInt(10) + BigInt(dividend.digits[i]);
            
            int count = 0;
            while (remainder >= divisor) {
                remainder = remainder - divisor;
                count++;
            }
            quotient.digits[i] = count;
        }
        
        quotient.removeLeadingZeros();
        quotient.negative = (negative != other.negative) && !quotient.isZero();
        return quotient;
    }
    
    BigInt operator%(const BigInt& other) const {
        if (other.isZero()) {
            throw std::runtime_error("Modulo by zero");
        }
        
        BigInt dividend = abs();
        BigInt divisor = other.abs();
        
        if (dividend < divisor) {
            return negative ? other.abs() - dividend : dividend;
        }
        
        BigInt remainder(0);
        
        // Long division to find remainder
        for (int i = dividend.digits.size() - 1; i >= 0; i--) {
            remainder = remainder * BigInt(10) + BigInt(dividend.digits[i]);
            
            while (remainder >= divisor) {
                remainder = remainder - divisor;
            }
        }
        
        return negative && !remainder.isZero() ? divisor - remainder : remainder;
    }
    
    std::string toString() const {
        std::string result;
        if (negative && !isZero()) result += "-";
        
        for (int i = digits.size() - 1; i >= 0; i--) {
            result += (char)('0' + digits[i]);
        }
        
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const BigInt& bi) {
        return os << bi.toString();
    }
};