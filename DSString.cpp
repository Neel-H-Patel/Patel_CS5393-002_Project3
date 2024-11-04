#include "DSString.h"

// Default constructor
DSString::DSString() : data(nullptr), len(0) {}

// Constructor from C-string
DSString::DSString(const char *str) {
    if (str) {
        len = 0;
        while (str[len] != '\0') {
            ++len;
        }
        data = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            data[i] = str[i];
        }
        data[len] = '\0';
    } else {
        data = nullptr;
        len = 0;
    }
}

// Copy constructor
DSString::DSString(const DSString &other) {
    len = other.len;
    if (other.data) {
        data = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            data[i] = other.data[i];
        }
        data[len] = '\0';
    } else {
        data = nullptr;
    }
}

// Copy assignment operator
DSString &DSString::operator=(const DSString &other) {
    if (this != &other) {
        delete[] data;
        len = other.len;
        if (other.data) {
            data = new char[len + 1];
            for (size_t i = 0; i < len; ++i) {
                data[i] = other.data[i];
            }
            data[len] = '\0';
        } else {
            data = nullptr;
        }
    }
    return *this;
}

// Destructor
DSString::~DSString() {
    delete[] data;
}

// Length of the string
size_t DSString::length() const {
    return len;
}

// Index operator
char &DSString::operator[](size_t index) {
    return data[index];
}

// Const version of index operator
const char &DSString::operator[](size_t index) const {
    return data[index];
}

// Concatenation operator
DSString DSString::operator+(const DSString &other) const {
    size_t newLen = len + other.len;
    char *newData = new char[newLen + 1];

    for (size_t i = 0; i < len; ++i) {
        newData[i] = data[i];
    }
    for (size_t i = 0; i < other.len; ++i) {
        newData[len + i] = other.data[i];
    }
    newData[newLen] = '\0';

    DSString result(newData);
    delete[] newData;
    return result;
}

// Equality operator
bool DSString::operator==(const DSString &other) const {
    if (len != other.len) return false;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

// Less than operator (lexicographical comparison)
bool DSString::operator<(const DSString &other) const {
    size_t minLen = (len < other.len) ? len : other.len;
    for (size_t i = 0; i < minLen; ++i) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return len < other.len;
}

// Substring method
DSString DSString::substring(size_t start, size_t numChars) const {
    if (start >= len) return DSString("");
    size_t actualLen = (start + numChars > len) ? len - start : numChars;
    char *subData = new char[actualLen + 1];
    for (size_t i = 0; i < actualLen; ++i) {
        subData[i] = data[start + i];
    }
    subData[actualLen] = '\0';
    DSString subString(subData);
    delete[] subData;
    return subString;
}

// Convert to lowercase
DSString DSString::toLower() const {
    DSString lowerStr(*this);
    for (size_t i = 0; i < len; ++i) {
        if (lowerStr.data[i] >= 'A' && lowerStr.data[i] <= 'Z') {
            lowerStr.data[i] += ('a' - 'A'); // Convert to lowercase
        }
    }
    return lowerStr;
}

// Return C-string
const char *DSString::c_str() const {
    return data;
}

// Output stream operator
std::ostream &operator<<(std::ostream &os, const DSString &str) {
    if (str.data) {
        os << str.data;
    }
    return os;
}
