#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>
#include <cstddef> // For size_t

class DSString
{
private:
    char *data;
    size_t len;

public:
    DSString();
    DSString(const char *);
    DSString(const DSString &);
    DSString &operator=(const DSString &);
    ~DSString();

    size_t length() const;
    char &operator[](size_t);
    const char &operator[](size_t) const; // Added const version

    DSString operator+(const DSString &) const;
    bool operator==(const DSString &) const;
    bool operator<(const DSString &) const;

    DSString substring(size_t start, size_t numChars) const;
    DSString toLower() const;

    const char *c_str() const;

    friend std::ostream &operator<<(std::ostream &, const DSString &);
};

// Specialization of std::hash for DSString
#include <functional>

namespace std {
    template<>
    struct hash<DSString> {
        std::size_t operator()(const DSString& s) const noexcept {
            std::size_t h = 0;
            const char* data = s.c_str();
            for (size_t i = 0; i < s.length(); ++i) {
                h = h * 31 + data[i];
            }
            return h;
        }
    };
}

#endif
