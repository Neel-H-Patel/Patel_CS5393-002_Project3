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
