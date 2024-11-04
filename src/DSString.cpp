#include "DSString.h"
#include <cstring>  // Only used in constructor to get string length

DSString::DSString() : data(nullptr), length(0) {
    data = new char[1];
    data[0] = '\0';
}

DSString::DSString(const char* str) {
    if (str == nullptr) {
        data = new char[1];
        data[0] = '\0';
        length = 0;
        return;
    }
    
    length = 0;
    while (str[length] != '\0') length++;
    
    data = new char[length + 1];
    for (size_t i = 0; i < length; i++) {
        data[i] = str[i];
    }
    data[length] = '\0';
}

DSString::DSString(const DSString& str) : length(str.length) {
    data = new char[length + 1];
    for (size_t i = 0; i <= length; i++) {
        data[i] = str.data[i];
    }
}

DSString::~DSString() {
    delete[] data;
}

DSString& DSString::operator=(const DSString& str) {
    if (this != &str) {
        delete[] data;
        length = str.length;
        data = new char[length + 1];
        for (size_t i = 0; i <= length; i++) {
            data[i] = str.data[i];
        }
    }
    return *this;
}

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

bool DSString::operator==(const DSString& str) const {
    if (length != str.length) return false;
    for (size_t i = 0; i < length; i++) {
        if (data[i] != str.data[i]) return false;
    }
    return true;
}

bool DSString::operator<(const DSString& str) const {
    size_t minLen = length < str.length ? length : str.length;
    for (size_t i = 0; i < minLen; i++) {
        if (data[i] < str.data[i]) return true;
        if (data[i] > str.data[i]) return false;
    }
    return length < str.length;
}

bool DSString::operator>(const DSString& str) const {
    return str < *this;
}

char& DSString::operator[](size_t index) {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

const char& DSString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

size_t DSString::getLength() const {
    return length;
}

const char* DSString::c_str() const {
    return data;
}

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

std::ostream& operator<<(std::ostream& out, const DSString& str) {
    out << str.data;
    return out;
} 