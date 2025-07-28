#pragma once
#include "fraction.h"
#include "bigint.h"
#include <vector>
#include <utility>
#include <stdexcept>

class LagrangeInterpolation {
public:
    static BigInt interpolateAtZero(const std::vector<std::pair<BigInt, BigInt>>& points) {
        if (points.empty()) {
            return BigInt(0);
        }
        
        Fraction result(0);
        
        for (size_t i = 0; i < points.size(); i++) {
            Fraction term(points[i].second); // y_i
            
            // Calculate the Lagrange basis polynomial L_i(0)
            for (size_t j = 0; j < points.size(); j++) {
                if (i != j) {
                    // L_i(0) *= (0 - x_j) / (x_i - x_j)
                    // Which simplifies to: (-x_j) / (x_i - x_j)
                    
                    BigInt numeratorVal = BigInt(0) - points[j].first; // -x_j
                    BigInt denominatorVal = points[i].first - points[j].first; // x_i - x_j
                    
                    if (denominatorVal.isZero()) {
                        throw std::runtime_error("Duplicate x values in interpolation points");
                    }
                    
                    Fraction factor(numeratorVal, denominatorVal);
                    term = term * factor;
                }
            }
            
            result = result + term;
        }
        
        if (!result.isInteger()) {
            throw std::runtime_error("Result is not an integer - invalid polynomial");
        }
        
        return result.toInteger();
    }
};