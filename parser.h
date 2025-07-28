#pragma once
#include "bigint.h"
#include "utils.h"
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>

struct TestCase {
    int n, k;
    std::vector<std::pair<BigInt, BigInt>> points;
};

class Parser {
private:
    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t\n\r");
        return str.substr(start, end - start + 1);
    }
    
    static std::string extractValue(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return "";
        
        pos = json.find("\"", pos);
        if (pos == std::string::npos) return "";
        
        size_t start = pos + 1;
        size_t end = json.find("\"", start);
        if (end == std::string::npos) return "";
        
        return json.substr(start, end - start);
    }
    
    static int extractIntValue(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return 0;
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return 0;
        
        // Skip whitespace
        pos++;
        while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        
        std::string numStr;
        while (pos < json.length() && json[pos] >= '0' && json[pos] <= '9') {
            numStr += json[pos];
            pos++;
        }
        
        return numStr.empty() ? 0 : std::stoi(numStr);
    }
    
public:
    static TestCase parseFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();
        file.close();
        
        return parseFromString(json);
    }
    
    static TestCase parseFromString(const std::string& json) {
        TestCase testCase;
        
        // Extract n and k
        testCase.n = extractIntValue(json, "n");
        testCase.k = extractIntValue(json, "k");
        
        // Extract points
        for (int i = 1; i <= testCase.n; i++) {
            std::string iStr = std::to_string(i);
            
            // Find the section for this point
            std::string searchPattern = "\"" + iStr + "\"";
            size_t pos = json.find(searchPattern);
            if (pos == std::string::npos) continue;
            
            // Find the opening brace for this object
            pos = json.find("{", pos);
            if (pos == std::string::npos) continue;
            
            // Find the closing brace
            size_t braceCount = 1;
            size_t start = pos + 1;
            size_t end = start;
            
            while (end < json.length() && braceCount > 0) {
                if (json[end] == '{') braceCount++;
                else if (json[end] == '}') braceCount--;
                end++;
            }
            
            if (braceCount == 0) {
                std::string objectJson = json.substr(start, end - start - 1);
                
                std::string baseStr = extractValue(objectJson, "base");
                std::string valueStr = extractValue(objectJson, "value");
                
                if (!baseStr.empty() && !valueStr.empty()) {
                    int base = std::stoi(baseStr);
                    BigInt x(i);
                    BigInt y = Utils::convertFromBase(valueStr, base);
                    testCase.points.push_back(std::make_pair(x, y));
                }
            }
        }
        
        return testCase;
    }
};