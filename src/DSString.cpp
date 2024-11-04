#include "DSString.h"
#include <cstring>  // Only used in constructor to get string length

/**
 * @brief Default constructor creates an empty string
 * Allocates minimum space for null terminator
 */
DSString::DSString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}

/**
 * @brief Constructs string from C-string
 * @param str Source C-string (can be nullptr)
 * Handles null pointer by creating empty string
 */
DSString::DSString(const char* str) {
    if (str == nullptr) {
        data = new char[1];
        data[0] = '\0';
        length = 0;
        return;
    }
    
    // Calculate length manually to avoid cstring functions
    length = 0;
    while (str[length] != '\0') length++;
    
    // Allocate and copy characters
    data = new char[length + 1];
    for (size_t i = 0; i < length; i++) {
        data[i] = str[i];
    }
    data[length] = '\0';
}

/**
 * @brief Copy constructor
 * @param str Source DSString to copy from
 * Creates deep copy of the source string
 */
DSString::DSString(const DSString& str) : length(str.length) {
    data = new char[length + 1];
    for (size_t i = 0; i <= length; i++) {
        data[i] = str.data[i];
    }
}

/**
 * @brief Destructor
 * Frees dynamically allocated memory
 */
DSString::~DSString() {
    delete[] data;
}

/**
 * @brief Assignment operator
 * @param str Source string to copy from
 * @return Reference to this object
 * Implements deep copy with self-assignment check
 */
DSString& DSString::operator=(const DSString& str) {
    if (this != &str) {  // Self-assignment check
        delete[] data;
        length = str.length;
        data = new char[length + 1];
        for (size_t i = 0; i <= length; i++) {
            data[i] = str.data[i];
        }
    }
    return *this;
}

/**
 * @brief String concatenation
 * @param str String to append
 * @return New string containing concatenated result
 * Creates new string with combined content
 */
DSString DSString::operator+(const DSString& str) const {
    char* newData = new char[length + str.length + 1];
    for (size_t i = 0; i < length; i++) {
        newData[i] = data[i];
    }
    for (size_t i = 0; i <= str.length; i++) {
        newData[length + i] = str.data[i];
    }
    DSString result;
    delete[] result.data;
    result.data = newData;
    result.length = length + str.length;
    return result;
}

/**
 * @brief Equality comparison
 * @param str String to compare with
 * @return true if strings are identical
 */
bool DSString::operator==(const DSString& str) const {
    if (length != str.length) return false;
    for (size_t i = 0; i < length; i++) {
        if (data[i] != str.data[i]) return false;
    }
    return true;
}

/**
 * @brief Less than comparison
 * @param str String to compare with
 * @return true if this string is lexicographically less than str
 */
bool DSString::operator<(const DSString& str) const {
    size_t minLen = length < str.length ? length : str.length;
    for (size_t i = 0; i < minLen; i++) {
        if (data[i] < str.data[i]) return true;
        if (data[i] > str.data[i]) return false;
    }
    return length < str.length;
}

/**
 * @brief Greater than comparison
 * @param str String to compare with
 * @return true if this string is lexicographically greater than str
 */
bool DSString::operator>(const DSString& str) const {
    return str < *this;
}

/**
 * @brief Array subscript operator
 * @param index Position to access
 * @return Reference to character at position
 * @throws std::out_of_range if index is invalid
 */
char& DSString::operator[](size_t index) {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

/**
 * @brief Const array subscript operator
 * @param index Position to access
 * @return Const reference to character at position
 * @throws std::out_of_range if index is invalid
 */
const char& DSString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

/**
 * @brief Get string length
 * @return Number of characters (excluding null terminator)
 */
size_t DSString::getLength() const {
    return length;
}

/**
 * @brief Get underlying C-string
 * @return Pointer to null-terminated character array
 */
const char* DSString::c_str() const {
    return data;
}

/**
 * @brief Extract substring
 * @param start Starting position
 * @param numChars Number of characters to extract
 * @return New string containing the substring
 */
DSString DSString::substring(size_t start, size_t numChars) const {
    if (start >= length) {
        return DSString();
    }
    
    if (start + numChars > length) {
        numChars = length - start;
    }
    
    char* newStr = new char[numChars + 1];
    for (size_t i = 0; i < numChars; i++) {
        newStr[i] = data[start + i];
    }
    newStr[numChars] = '\0';
    
    DSString result(newStr);
    delete[] newStr;
    return result;
}

/**
 * @brief Stream output operator
 * @param out Output stream
 * @param str String to output
 * @return Reference to output stream
 */
std::ostream& operator<<(std::ostream& out, const DSString& str) {
    out << str.data;
    return out;
} 