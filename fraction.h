#pragma once
#include "bigint.h"
#include "utils.h"
#include <stdexcept>

class Fraction {
private:
    BigInt numerator;
    BigInt denominator;
    
    void simplify() {
        if (denominator.isZero()) {
            throw std::runtime_error("Division by zero");
        }
        
        BigInt g = Utils::gcd(numerator.abs(), denominator.abs());
        if (!g.isZero() && g != BigInt(1)) {
            numerator = numerator / g;
            denominator = denominator / g;
        }
        
        if (denominator.isNegative()) {
            numerator = BigInt(0) - numerator;
            denominator = denominator.abs();
        }
    }
    
public:
    Fraction() : numerator(0), denominator(1) {}
    
    Fraction(const BigInt& num) : numerator(num), denominator(1) {}
    
    Fraction(const BigInt& num, const BigInt& den) : numerator(num), denominator(den) {
        simplify();
    }
    
    Fraction operator+(const Fraction& other) const {
        BigInt newNum = numerator * other.denominator + other.numerator * denominator;
        BigInt newDen = denominator * other.denominator;
        return Fraction(newNum, newDen);
    }
    
    Fraction operator-(const Fraction& other) const {
        BigInt newNum = numerator * other.denominator - other.numerator * denominator;
        BigInt newDen = denominator * other.denominator;
        return Fraction(newNum, newDen);
    }
    
    Fraction operator*(const Fraction& other) const {
        return Fraction(numerator * other.numerator, denominator * other.denominator);
    }
    
    Fraction operator/(const Fraction& other) const {
        if (other.numerator.isZero()) {
            throw std::runtime_error("Division by zero");
        }
        return Fraction(numerator * other.denominator, denominator * other.numerator);
    }
    
    BigInt toInteger() const {
        return numerator / denominator;
    }
    
    bool isInteger() const {
        return denominator == BigInt(1) || (numerator % denominator).isZero();
    }
    
    std::string toString() const {
        if (denominator == BigInt(1)) {
            return numerator.toString();
        }
        return numerator.toString() + "/" + denominator.toString();
    }
};