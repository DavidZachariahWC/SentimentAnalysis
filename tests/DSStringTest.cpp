#include "DSString.h"
#include <cassert>
#include <iostream>

void testConstructors() {
    // Default constructor
    DSString s1;
    assert(s1.getLength() == 0);
    assert(strcmp(s1.c_str(), "") == 0);
    
    // C-string constructor
    DSString s2("hello");
    assert(s2.getLength() == 5);
    assert(strcmp(s2.c_str(), "hello") == 0);
    
    // Copy constructor
    DSString s3(s2);
    assert(s3.getLength() == s2.getLength());
    assert(strcmp(s3.c_str(), s2.c_str()) == 0);
    
    std::cout << "Constructor tests passed!" << std::endl;
}

void testOperators() {
    DSString s1("hello");
    DSString s2("world");
    
    // Assignment operator
    DSString s3;
    s3 = s1;
    assert(strcmp(s3.c_str(), "hello") == 0);
    
    // Concatenation
    DSString s4 = s1 + s2;
    assert(strcmp(s4.c_str(), "helloworld") == 0);
    
    // Comparison operators
    assert(!(s1 == s2));
    assert(s1 < s2);  // 'h' comes before 'w'
    assert(s2 > s1);
    
    // Subscript operator
    assert(s1[0] == 'h');
    assert(s1[4] == 'o');
    
    std::cout << "Operator tests passed!" << std::endl;
}

void testUtilityFunctions() {
    DSString s1("Hello World!");
    
    // getLength
    assert(s1.getLength() == 12);
    
    // substring
    DSString sub = s1.substring(0, 5);
    assert(strcmp(sub.c_str(), "Hello") == 0);
    
    // Test substring with invalid parameters
    DSString empty = s1.substring(100, 5);  // Should return empty string
    assert(empty.getLength() == 0);
    
    std::cout << "Utility function tests passed!" << std::endl;
}

void testEdgeCases() {
    // Empty string
    DSString s1("");
    assert(s1.getLength() == 0);
    
    // Single character
    DSString s2("a");
    assert(s2.getLength() == 1);
    
    // Long string
    std::string longStr(1000, 'a');  // String of 1000 'a' characters
    DSString s3(longStr.c_str());
    assert(s3.getLength() == 1000);
    
    // Null pointer
    DSString s4(nullptr);
    assert(s4.getLength() == 0);
    
    std::cout << "Edge case tests passed!" << std::endl;
}

int main() {
    try {
        std::cout << "Starting DSString tests..." << std::endl;
        
        testConstructors();
        testOperators();
        testUtilityFunctions();
        testEdgeCases();
        
        std::cout << "\nAll tests passed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}