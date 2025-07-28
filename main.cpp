#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "bigint.h"
#include "fraction.h"
#include "interpolation.h"
#include "parser.h"
#include "utils.h"

int main() {
    try {
        // Test Case 1
        std::cout << "=== Test Case 1 ===" << std::endl;
        TestCase testCase1 = Parser::parseFromFile("input.json");
        
        std::cout << "n = " << testCase1.n << ", k = " << testCase1.k << std::endl;
        std::cout << "Points:" << std::endl;
        
        for (const auto& point : testCase1.points) {
            std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
        }
        
        // Use only the first k points for interpolation
        std::vector<std::pair<BigInt, BigInt>> interpolationPoints1;
        for (int i = 0; i < std::min(testCase1.k, (int)testCase1.points.size()); i++) {
            interpolationPoints1.push_back(testCase1.points[i]);
        }
        
        BigInt secret1 = LagrangeInterpolation::interpolateAtZero(interpolationPoints1);
        std::cout << "Secret (constant term): " << secret1 << std::endl << std::endl;
        
        // Test Case 2
        std::cout << "=== Test Case 2 ===" << std::endl;
        std::string testCase2Json = R"({
            "keys": {
                "n": 10,
                "k": 7
            },
            "1": {
                "base": "6",
                "value": "13444211440455345511"
            },
            "2": {
                "base": "15",
                "value": "aed7015a346d63"
            },
            "3": {
                "base": "15",
                "value": "6aeeb69631c227c"
            },
            "4": {
                "base": "16",
                "value": "e1b5e05623d881f"
            },
            "5": {
                "base": "8",
                "value": "316034514573652620673"
            },
            "6": {
                "base": "3",
                "value": "2122212201122002221120200210011020220200"
            },
            "7": {
                "base": "3",
                "value": "20120221122211000100210021102001201112121"
            },
            "8": {
                "base": "6",
                "value": "20220554335330240002224253"
            },
            "9": {
                "base": "12",
                "value": "45153788322a1255483"
            },
            "10": {
                "base": "7",
                "value": "1101613130313526312514143"
            }
        })";
        
        TestCase testCase2 = Parser::parseFromString(testCase2Json);
        
        std::cout << "n = " << testCase2.n << ", k = " << testCase2.k << std::endl;
        std::cout << "Points:" << std::endl;
        
        for (const auto& point : testCase2.points) {
            std::cout << "(" << point.first << ", " << point.second << ")" << std::endl;
        }
        
        // Use only the first k points for interpolation
        std::vector<std::pair<BigInt, BigInt>> interpolationPoints2;
        for (int i = 0; i < std::min(testCase2.k, (int)testCase2.points.size()); i++) {
            interpolationPoints2.push_back(testCase2.points[i]);
        }
        
        BigInt secret2 = LagrangeInterpolation::interpolateAtZero(interpolationPoints2);
        std::cout << "Secret (constant term): " << secret2 << std::endl;
        
        // Summary
        std::cout << std::endl << "=== RESULTS ===" << std::endl;
        std::cout << "Test Case 1 Secret: " << secret1 << std::endl;
        std::cout << "Test Case 2 Secret: " << secret2 << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}