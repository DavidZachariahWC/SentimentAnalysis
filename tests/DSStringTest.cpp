#include "DSString.h"
#include <cassert>
#include <iostream>

/**
 * Tests all constructors: default, c-string, and copy constructor
 * Verifies proper string creation and deep copying
 */
void testConstructors() {
    DSString s1;  // Default constructor
    assert(s1.getLength() == 0);
    assert(strcmp(s1.c_str(), "") == 0);
    
    DSString s2("hello");  // C-string constructor
    assert(s2.getLength() == 5);
    assert(strcmp(s2.c_str(), "hello") == 0);
    
    DSString s3(s2);  // Copy constructor
    assert(s3.getLength() == s2.getLength());
    assert(strcmp(s3.c_str(), s2.c_str()) == 0);
    
    std::cout << "Constructor tests passed!" << std::endl;
}

/**
 * Tests operator overloads: assignment, concatenation, comparison, and subscript
 * Verifies correct operator behavior and memory management
 */
void testOperators() {
    DSString s1("hello");
    DSString s2("world");
    
    DSString s3;
    s3 = s1;  // Assignment
    assert(strcmp(s3.c_str(), "hello") == 0);
    
    DSString s4 = s1 + s2;  // Concatenation
    assert(strcmp(s4.c_str(), "helloworld") == 0);
    
    // Comparison operators
    assert(!(s1 == s2));
    assert(s1 < s2);
    assert(s2 > s1);
    
    // Subscript operator
    assert(s1[0] == 'h');
    assert(s1[4] == 'o');
    
    std::cout << "Operator tests passed!" << std::endl;
}

/**
 * Tests utility functions: getLength, substring
 * Verifies correct string manipulation and boundary handling
 */
void testUtilityFunctions() {
    DSString s1("Hello World!");
    
    assert(s1.getLength() == 12);
    
    DSString sub = s1.substring(0, 5);
    assert(strcmp(sub.c_str(), "Hello") == 0);
    
    // Test invalid parameters
    DSString empty = s1.substring(100, 5);
    assert(empty.getLength() == 0);
    
    std::cout << "Utility function tests passed!" << std::endl;
}

/**
 * Tests edge cases: empty strings, single chars, long strings, and null pointers
 * Verifies robust handling of special cases
 */
void testEdgeCases() {
    DSString s1("");
    assert(s1.getLength() == 0);
    
    DSString s2("a");
    assert(s2.getLength() == 1);
    
    std::string longStr(1000, 'a');
    DSString s3(longStr.c_str());
    assert(s3.getLength() == 1000);
    
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