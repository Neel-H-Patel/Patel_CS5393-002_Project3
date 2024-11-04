// Name: Neel Patel
// Class: Data Structures CS5393-002
// Assignment: Project #3

// DESIGN DOCUMENTATION

/*
 
This project implements a Sentiment Analysis classifier that classifies tweets as positive or negative based on their content. 
The classifier is built using a custom string class (`DSString`), dynamic memory allocation, and adheres to the rule-of-three 
for memory management. The program processes training data to build a sentiment model and then uses this model to predict sentiments 
of testing tweets. It evaluates the classifier's performance by comparing predictions against ground truth data.

Objectives:

- Implement a custom string class (`DSString`) with dynamic memory management.
- Develop a `SentimentClassifier` class to handle training, prediction, and evaluation.
- Read and process datasets from CSV files using command-line arguments.
- Utilize STL containers such as `std::vector` and `std::unordered_map` for efficient data storage and retrieval.
- Ensure proper memory management and exception handling.
- Analyze runtime complexity and optimize for performance.
- Provide meaningful output files with classifier results and accuracy metrics.

Design Overview:

- DSString Class:
  - Custom string class managing a dynamic character array.
  - Implements constructors, destructor, copy constructor, and copy assignment operator (rule-of-three).
  - Provides necessary string operations without using STL `std::string` internally.

- SentimentClassifier Class:
  - Handles training by analyzing word frequencies in positive and negative tweets.
  - Predicts sentiment of new tweets based on trained word frequencies.
  - Evaluates predictions against ground truth to calculate accuracy.
  - Utilizes `std::unordered_map` to store word counts for efficient lookup.

- Main Function:
  - Parses command-line arguments to obtain file paths.
  - Reads training data to build the sentiment model.
  - Processes testing data to generate sentiment predictions.
  - Compares predictions with ground truth to compute accuracy.
  - Writes results and accuracy metrics to output files.

Challenges and Solutions:

- Custom String Management:
  - Implemented `DSString` with careful handling of dynamic memory to avoid leaks.
  - Ensured deep copying in copy constructor and assignment operator.

- Efficient Data Processing:
  - Used `std::unordered_map` for constant-time word frequency lookups.
  - Optimized file I/O operations to handle large datasets effectively.

- Memory Management:
  - Adhered to the rule-of-three in `DSString` to manage dynamic memory.
  - Implemented destructors to clean up allocated resources.

- Exception Handling:
  - Incorporated try-catch blocks to handle file I/O errors and invalid data gracefully.

*/

// Collaborators: None

// References:
// https://en.cppreference.com/w/cpp/container/unordered_map
// https://en.cppreference.com/w/cpp/string/basic_string
// https://www.cplusplus.com/reference/vector/vector/
// https://www.geeksforgeeks.org/file-input-output-stream-cpp/
// https://www.geeksforgeeks.org/rule-three-cpp/

// import necessary libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <exception>
#include <cstring> // Only for temporary usage outside DSString

using namespace std;

// DSString Class Implementation
class DSString {
private:
    char* data;
    int len;

public:
    // Default Constructor
    DSString() : data(nullptr), len(0) {}

    // Parameterized Constructor
    DSString(const char* s) {
        if (s) {
            len = strlen(s);
            data = new char[len + 1];
            strcpy(data, s);
        } else {
            data = nullptr;
            len = 0;
        }
    }

    // Copy Constructor
    DSString(const DSString& other) {
        len = other.len;
        if (other.data) {
            data = new char[len + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
        }
    }

    // Copy Assignment Operator
    DSString& operator=(const DSString& other) {
        if (this == &other)
            return *this;

        delete[] data;

        len = other.len;
        if (other.data) {
            data = new char[len + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
        }
        return *this;
    }

    // Destructor
    ~DSString() {
        delete[] data;
    }

    // Get Length
    int length() const {
        return len;
    }

    // Get C-String
    const char* c_str() const {
        return data;
    }

    // Convert to lowercase
    DSString toLower() const {
        DSString lowerStr;
        lowerStr.len = len;
        lowerStr.data = new char[len + 1];
        for (int i = 0; i < len; ++i) {
            lowerStr.data[i] = tolower(data[i]);
        }
        lowerStr.data[len] = '\0';
        return lowerStr;
    }

    // Overload += operator for concatenation
    DSString& operator+=(const DSString& other) {
        char* newData = new char[len + other.len + 1];
        if (data) {
            strcpy(newData, data);
        }
        if (other.data) {
            strcpy(newData + len, other.data);
        }
        delete[] data;
        data = newData;
        len += other.len;
        return *this;
    }

    // Overload << operator for output
    friend ostream& operator<<(ostream& os, const DSString& s) {
        if (s.data)
            os << s.data;
        return os;
    }

    // Overload == operator for comparison
    bool operator==(const DSString& other) const {
        if (len != other.len)
            return false;
        if (data == nullptr && other.data == nullptr)
            return true;
        if (data == nullptr || other.data == nullptr)
            return false;
        return strcmp(data, other.data) == 0;
    }
};


// Time and Space Complexity Analysis

/*
1. DSString Class:

   c. Time complexity of operations in (i) and (ii):
      - (i) Inserting words into the trie: O(m) per insertion, where m is the length of the word.
      - (ii) Searching for a word in the trie: O(m) per search.

   d. Time complexity of operations in (iii) and (iv):
      - (iii) Finding words with a given prefix: O(p + w), where p is the length of the prefix and w is the number of words with that prefix.
      - (iv) Searching for short words: O(k) per search, where k is the length of the short word.

   e. Space complexity of the trie of n words:
      - O(N * M), where N is the number of words and M is the average length of the words.
      - Each node can have up to 26 children (for each alphabet letter), but space is only allocated for characters that are used.

2. SentimentClassifier:

   c. Time complexity of operations in (i) and (ii):
      - (i) Training the classifier: O(T * W), where T is the number of training tweets and W is the average number of words per tweet.
      - (ii) Predicting sentiment for a tweet: O(W), where W is the number of words in the tweet.

   d. Time complexity of operations in (iii) and (iv):
      - (iii) Evaluating predictions: O(E), where E is the number of testing tweets.
      - (iv) Writing results and accuracy: O(E).

   e. Space complexity of the SentimentClassifier:
      - O(V), where V is the number of unique words in the training dataset.
      - Uses unordered_maps to store word counts for positive and negative sentiments.

Overall, the classifier's performance is primarily influenced by the number of tweets and the vocabulary size. Efficient use of 
unordered_maps ensures that word frequency lookups are performed in constant time on average, contributing to the overall efficiency of the system.

*/
