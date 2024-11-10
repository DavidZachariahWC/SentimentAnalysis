#include "DSString.h"
#include <cstring>  // Used only in constructor to determine the length of the string

/**
 * @brief Default constructor initializes an empty string
 * Allocates memory for the null terminator
 */
DSString::DSString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}

/**
 * @brief Constructs a DSString from a C-string
 * @param str The source C-string (can be nullptr)
 * If the input C-string is nullptr, an empty string is created
 */
DSString::DSString(const char* str) {
    if (str == nullptr) {
        data = new char[1];
        data[0] = '\0';
        length = 0;
        return;
    }
    
    // Manually calculate the length of the C-string to avoid using C++ string functions
    length = 0;
    while (str[length] != '\0') length++;
    
    // Allocate memory for the string and copy the characters
    data = new char[length + 1];
    for (size_t i = 0; i < length; i++) {
        data[i] = str[i];
    }
    data[length] = '\0';
}

/**
 * @brief Copy constructor for creating a deep copy of another DSString
 * @param str The source DSString to copy from
 */
DSString::DSString(const DSString& str) : length(str.length) {
    data = new char[length + 1];
    for (size_t i = 0; i <= length; i++) {
        data[i] = str.data[i];
    }
}

/**
 * @brief Destructor that frees the dynamically allocated memory
 */
DSString::~DSString() {
    delete[] data;
}

/**
 * @brief Assignment operator that performs a deep copy
 * @param str The source DSString to copy from
 * @return A reference to this DSString object
 * Includes a self-assignment check to avoid overwriting the object with itself
 */
DSString& DSString::operator=(const DSString& str) {
    if (this != &str) {  // Check for self-assignment
        delete[] data;   // Delete any previously allocated memory
        length = str.length;
        data = new char[length + 1];
        for (size_t i = 0; i <= length; i++) {
            data[i] = str.data[i];
        }
    }
    return *this;
}

/**
 * @brief Concatenation operator for appending another string to this one
 * @param str The string to append
 * @return A new DSString that contains the concatenated result
 */
DSString DSString::operator+(const DSString& str) const {
    char* newData = new char[length + str.length + 1];  // Allocate memory for the new string
    for (size_t i = 0; i < length; i++) {
        newData[i] = data[i];  // Copy the current string's data
    }
    for (size_t i = 0; i <= str.length; i++) {
        newData[length + i] = str.data[i];  // Copy the new string's data
    }
    DSString result;
    delete[] result.data;  // Deallocate any existing memory for the result
    result.data = newData;
    result.length = length + str.length;
    return result;
}

/**
 * @brief Equality comparison operator
 * @param str The string to compare with
 * @return true if the strings are identical, otherwise false
 */
bool DSString::operator==(const DSString& str) const {
    if (length != str.length) return false;  // If lengths differ, the strings are not equal
    for (size_t i = 0; i < length; i++) {
        if (data[i] != str.data[i]) return false;  // Compare each character
    }
    return true;
}

/**
 * @brief Less-than comparison operator
 * @param str The string to compare with
 * @return true if this string is lexicographically less than the other
 */
bool DSString::operator<(const DSString& str) const {
    size_t minLen = length < str.length ? length : str.length;
    for (size_t i = 0; i < minLen; i++) {
        if (data[i] < str.data[i]) return true;
        if (data[i] > str.data[i]) return false;
    }
    return length < str.length;  // If all characters match, the shorter string is considered less
}

/**
 * @brief Greater-than comparison operator
 * @param str The string to compare with
 * @return true if this string is lexicographically greater than the other
 */
bool DSString::operator>(const DSString& str) const {
    return str < *this;  // Use the less-than operator to determine greater-than
}

/**
 * @brief Subscript operator for accessing individual characters
 * @param index The position of the character to access
 * @return A reference to the character at the specified position
 * @throws std::out_of_range if the index is invalid
 */
char& DSString::operator[](size_t index) {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");  // Throw exception if index is out of range
    }
    return data[index];
}

/**
 * @brief Const subscript operator for accessing characters in a constant object
 * @param index The position of the character to access
 * @return A const reference to the character at the specified position
 * @throws std::out_of_range if the index is invalid
 */
const char& DSString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");  // Throw exception if index is out of range
    }
    return data[index];
}

/**
 * @brief Retrieves the length of the string
 * @return The number of characters in the string (excluding the null terminator)
 */
size_t DSString::getLength() const {
    return length;
}

/**
 * @brief Retrieves the underlying C-string
 * @return A pointer to the null-terminated character array
 */
const char* DSString::c_str() const {
    return data;
}

/**
 * @brief Extracts a substring from this string
 * @param start The starting position of the substring
 * @param numChars The number of characters to extract
 * @return A new DSString containing the extracted substring
 */
DSString DSString::substring(size_t start, size_t numChars) const {
    if (start >= length) {
        return DSString();  // Return an empty string if the starting index is invalid
    }
    
    if (start + numChars > length) {
        numChars = length - start;  // Adjust numChars if it exceeds the string length
    }
    
    char* newStr = new char[numChars + 1];  // Allocate memory for the substring
    for (size_t i = 0; i < numChars; i++) {
        newStr[i] = data[start + i];  // Copy the substring
    }
    newStr[numChars] = '\0';  // Add the null terminator
    
    DSString result(newStr);  // Create a new DSString with the substring
    delete[] newStr;  // Clean up temporary memory
    return result;
}

/**
 * @brief Stream output operator to print a DSString
 * @param out The output stream
 * @param str The DSString to output
 * @return The output stream
 */
std::ostream& operator<<(std::ostream& out, const DSString& str) {
    out << str.data;  // Print the underlying character array of the DSString
    return out;
}
