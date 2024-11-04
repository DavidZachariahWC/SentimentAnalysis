#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>

class DSString {
private:
    char* data;      // Pointer to the character array
    size_t length;   // Length of the string (excluding null terminator)

public:
    // Constructors
    DSString();
    DSString(const char* str);
    DSString(const DSString& str);  // Copy constructor

    // Destructor
    ~DSString();

    // Assignment operator
    DSString& operator=(const DSString& str);

    // Concatenation
    DSString operator+(const DSString& str) const;

    // Comparison operators
    bool operator==(const DSString& str) const;
    bool operator<(const DSString& str) const;
    bool operator>(const DSString& str) const;

    // Subscript operator
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    // Utility functions
    size_t getLength() const;
    const char* c_str() const;  // Returns the underlying c-string
    DSString substring(size_t start, size_t numChars) const;
    
    // Friend function for stream output
    friend std::ostream& operator<<(std::ostream& out, const DSString& str);
};

#endif 