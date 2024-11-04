#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>

/**
 * @class DSString
 * @brief Custom string class implementation with dynamic memory management
 * 
 * This class provides basic string operations while managing its own memory.
 * Implements the Rule of Three: destructor, copy constructor, and copy assignment.
 */
class DSString {
private:
    char* data;      // Pointer to the dynamically allocated character array
    size_t length;   // Length of the string (excluding null terminator)

public:
    /**
     * @brief Default constructor creates an empty string
     * Time Complexity: O(1)
     */
    DSString();

    /**
     * @brief Constructor from C-string
     * @param str Null-terminated C-string to copy
     * Time Complexity: O(n) where n is string length
     */
    DSString(const char* str);

    /**
     * @brief Copy constructor
     * @param str DSString to copy from
     * Time Complexity: O(n) where n is string length
     */
    DSString(const DSString& str);

    /**
     * @brief Destructor to free allocated memory
     * Time Complexity: O(1)
     */
    ~DSString();

    /**
     * @brief Copy assignment operator
     * @param str DSString to copy from
     * @return Reference to this object
     * Time Complexity: O(n) where n is string length
     */
    DSString& operator=(const DSString& str);

    /**
     * @brief String concatenation operator
     * @param str String to append
     * @return New string containing concatenated result
     * Time Complexity: O(n + m) where n and m are string lengths
     */
    DSString operator+(const DSString& str) const;

    /**
     * @brief Equality comparison operator
     * @param str String to compare with
     * @return true if strings are identical
     * Time Complexity: O(n) where n is string length
     */
    bool operator==(const DSString& str) const;

    /**
     * @brief Less than comparison operator
     * @param str String to compare with
     * @return true if this string is lexicographically less than str
     * Time Complexity: O(n) where n is length of shorter string
     */
    bool operator<(const DSString& str) const;

    /**
     * @brief Greater than comparison operator
     * @param str String to compare with
     * @return true if this string is lexicographically greater than str
     * Time Complexity: O(n) where n is length of shorter string
     */
    bool operator>(const DSString& str) const;

    /**
     * @brief Array subscript operator
     * @param index Position to access
     * @return Reference to character at position
     * Time Complexity: O(1)
     * @throws std::out_of_range if index is invalid
     */
    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    /**
     * @brief Get string length
     * @return Number of characters (excluding null terminator)
     * Time Complexity: O(1)
     */
    size_t getLength() const;

    /**
     * @brief Get underlying C-string
     * @return Pointer to null-terminated character array
     * Time Complexity: O(1)
     */
    const char* c_str() const;

    /**
     * @brief Extract substring
     * @param start Starting position
     * @param numChars Number of characters to extract
     * @return New string containing the substring
     * Time Complexity: O(n) where n is numChars
     */
    DSString substring(size_t start, size_t numChars) const;
    
    /**
     * @brief Stream output operator
     * @param out Output stream
     * @param str String to output
     * @return Reference to output stream
     * Time Complexity: O(n) where n is string length
     */
    friend std::ostream& operator<<(std::ostream& out, const DSString& str);
};

/**
 * @brief Hash function specialization for DSString
 * 
 * Implements DJB2 hash algorithm for use in unordered containers
 * Time Complexity: O(n) where n is string length
 */
namespace std {
    template <>
    struct hash<DSString> {
        size_t operator()(const DSString& str) const {
            size_t hash = 5381;
            const char* ptr = str.c_str();
            while (*ptr) {
                hash = ((hash << 5) + hash) + (*ptr); // hash * 33 + c
                ptr++;
            }
            return hash;
        }
    };
}

#endif 