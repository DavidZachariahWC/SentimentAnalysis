#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>

/**
 * @class DSString
 * @brief A custom string class that handles dynamic memory management
 * 
 * This class provides essential string operations while managing memory dynamically.
 * It follows the Rule of Three: defining the destructor, copy constructor, and copy assignment operator.
 */
class DSString {
private:
    char* data;      // Pointer to the dynamically allocated memory for the string
    size_t length;   // The length of the string, excluding the null terminator

public:
    /**
     * @brief Default constructor creates an empty string.
     * Time Complexity: O(1)
     */
    DSString();

    /**
     * @brief Constructor that initializes the string from a C-string.
     * @param str A null-terminated C-string to be copied
     * Time Complexity: O(n), where n is the length of the string
     */
    DSString(const char* str);

    /**
     * @brief Copy constructor
     * @param str Another DSString object to copy from
     * Time Complexity: O(n), where n is the length of the string
     */
    DSString(const DSString& str);

    /**
     * @brief Destructor to release any dynamically allocated memory
     * Time Complexity: O(1)
     */
    ~DSString();

    /**
     * @brief Copy assignment operator
     * @param str Another DSString to copy from
     * @return A reference to the current object
     * Time Complexity: O(n), where n is the length of the string
     */
    DSString& operator=(const DSString& str);

    /**
     * @brief String concatenation operator
     * @param str The string to append
     * @return A new DSString containing the concatenated result
     * Time Complexity: O(n + m), where n and m are the lengths of the two strings
     */
    DSString operator+(const DSString& str) const;

    /**
     * @brief Equality comparison operator
     * @param str The string to compare to
     * @return true if the two strings are equal, false otherwise
     * Time Complexity: O(n), where n is the length of the string
     */
    bool operator==(const DSString& str) const;

    /**
     * @brief Less-than comparison operator
     * @param str The string to compare to
     * @return true if this string is lexicographically less than the other string
     * Time Complexity: O(n), where n is the length of the shorter string
     */
    bool operator<(const DSString& str) const;

    /**
     * @brief Greater-than comparison operator
     * @param str The string to compare to
     * @return true if this string is lexicographically greater than the other string
     * Time Complexity: O(n), where n is the length of the shorter string
     */
    bool operator>(const DSString& str) const;

    /**
     * @brief Array subscript operator for non-constant access
     * @param index The position of the character to access
     * @return A reference to the character at the specified index
     * Time Complexity: O(1)
     * @throws std::out_of_range if the index is out of bounds
     */
    char& operator[](size_t index);

    /**
     * @brief Array subscript operator for constant access
     * @param index The position of the character to access
     * @return A reference to the character at the specified index
     * Time Complexity: O(1)
     * @throws std::out_of_range if the index is out of bounds
     */
    const char& operator[](size_t index) const;

    /**
     * @brief Get the length of the string (excluding the null terminator)
     * @return The length of the string
     * Time Complexity: O(1)
     */
    size_t getLength() const;

    /**
     * @brief Get the underlying C-string (null-terminated array of characters)
     * @return A pointer to the null-terminated C-string
     * Time Complexity: O(1)
     */
    const char* c_str() const;

    /**
     * @brief Extract a substring from the string
     * @param start The starting index for the substring
     * @param numChars The number of characters to include in the substring
     * @return A new DSString object containing the substring
     * Time Complexity: O(n), where n is the number of characters in the substring
     */
    DSString substring(size_t start, size_t numChars) const;

    /**
     * @brief Stream output operator for printing the string
     * @param out The output stream
     * @param str The DSString object to output
     * @return The output stream
     * Time Complexity: O(n), where n is the length of the string
     */
    friend std::ostream& operator<<(std::ostream& out, const DSString& str);
};

/**
 * @brief Specialization of the hash function for DSString
 * 
 * Implements the DJB2 hash algorithm, which is commonly used for hashing strings
 * Time Complexity: O(n), where n is the length of the string
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
