#include "SentimentClassifier.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cctype>
#include <iomanip>
#include <chrono>
#include <unordered_set>

std::unordered_set<std::string> stopWords = {
    "a", "an", "and", "are", "as", "at", "be", "by",
    "for", "from", "has", "he", "in", "is", "it", "its",
    "of", "on", "that", "the", "to", "was", "were", "will", "with"
};

// Tokenizer function remains the same
std::vector<DSString> tokenize(const DSString& tweet) {
    std::vector<DSString> words;
    size_t start = 0;
    size_t len = tweet.length();

    while (start < len) {
        // Skip non-alphanumeric characters
        while (start < len && !std::isalnum(tweet[start])) {
            ++start;
        }

        // Find the end of the word
        size_t end = start;
        while (end < len && std::isalnum(tweet[end])) {
            ++end;
        }

        if (start < end) {
            DSString word = tweet.substring(start, end - start).toLower();
            std::string wordStr = word.c_str();

            if (stopWords.find(wordStr) == stopWords.end()) {
            words.push_back(word);
        }
            start = end;
        }
    }
    return words;
}

// Custom CSV parsing function
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);

    // Remove surrounding quotes from fields
    for (auto& f : fields) {
        if (f.length() >= 2 && f.front() == '"' && f.back() == '"') {
            f = f.substr(1, f.length() - 2);
        }
    }

    return fields;
}

void SentimentClassifier::train(const char* trainingDataFile) {
    auto start = std::chrono::steady_clock::now();  // Start timer

    std::ifstream infile(trainingDataFile);
    if (!infile.is_open()) {
        std::cerr << "Error opening training data file." << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;

    // Skip header line if present
    if (!infile.eof()) {
        std::getline(infile, line);
        if (line.find("Sentiment") != std::string::npos) {
            // Header detected, skip this line
        } else {
            // Reset to the beginning if no header
            infile.clear();
            infile.seekg(0, std::ios::beg);
        }
    }

    while (std::getline(infile, line)) {
        ++lineNumber;

        std::vector<std::string> fields = parseCSVLine(line);

        // Ensure we have at least 6 fields
        if (fields.size() < 6) {
            std::cerr << "Malformed line at " << lineNumber << ": " << line << std::endl;
            continue; // Skip this line
        }

        std::string sentimentStr = fields[0];
        std::string idStr = fields[1];
        // Other fields as needed...
        std::string textStr = fields[5];

        try {
            int sentiment = std::stoi(sentimentStr);
            DSString tweetText(textStr.c_str());

            // Tokenize tweet
            std::vector<DSString> words = tokenize(tweetText);

            // Update word counts
            if (sentiment == 4) {
                totalPositiveTweets++;
                for (const auto& word : words) {
                    positiveWordCounts[word]++;
                }
            } else if (sentiment == 0) {
                totalNegativeTweets++;
                for (const auto& word : words) {
                    negativeWordCounts[word]++;
                }
            }
            // Ignore tweets with other sentiment values
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid sentiment value at line " << lineNumber << ": '" << sentimentStr << "' in line: " << line << std::endl;
            continue; // Skip this line
        }
    }

    infile.close();

    auto end = std::chrono::steady_clock::now();  // End timer
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Training time: " << elapsed_seconds.count() << "s\n";
}

void SentimentClassifier::predict(const char* testingDataFile, const char* outputFile) {
    auto start = std::chrono::steady_clock::now();  // Start timer

    std::ifstream infile(testingDataFile);
    std::ofstream outfile(outputFile);
    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening testing data or output file." << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;

    // Skip header line if present
    if (!infile.eof()) {
        std::getline(infile, line);

        // Convert line to lowercase for case-insensitive comparison
        std::string lowerLine = line;
        std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

        if (lowerLine.find("id") != std::string::npos && lowerLine.find("tweet") != std::string::npos) {
            // Header detected, skip this line
        } else {
            // Reset to the beginning if no header
            infile.clear();
            infile.seekg(0, std::ios::beg);
        }
    }

    while (std::getline(infile, line)) {
        ++lineNumber;

        std::vector<std::string> fields = parseCSVLine(line);

        // Ensure we have at least 5 fields (testing data doesn't have sentiment)
        if (fields.size() < 5) {
            std::cerr << "Malformed line at " << lineNumber << ": " << line << std::endl;
            continue; // Skip this line
        }

        std::string idStr = fields[0];
        std::string textStr = fields[4];

        try {
            long long tweetID = std::stoll(idStr);
            DSString tweetText(textStr.c_str());

            // Tokenize tweet
            std::vector<DSString> words = tokenize(tweetText);

            // Predict sentiment
            int positiveScore = 0;
            int negativeScore = 0;

            for (const auto& word : words) {
                positiveScore += positiveWordCounts[word];
                negativeScore += negativeWordCounts[word];
            }

            int predictedSentiment = (positiveScore >= negativeScore) ? 4 : 0;

            // Write prediction to output file
            outfile << predictedSentiment << ", " << tweetID << "\n";
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid ID value at line " << lineNumber << ": '" << idStr << "' in line: " << line << std::endl;
            continue; // Skip this line
        }
    }

    infile.close();
    outfile.close();

    auto end = std::chrono::steady_clock::now();  // End timer
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Prediction time: " << elapsed_seconds.count() << "s\n";
}

void SentimentClassifier::evaluatePredictions(const char* predictionsFile, const char* groundTruthFile, const char* accuracyFile) {
    auto start = std::chrono::steady_clock::now();  // Start timer

    std::ifstream predFile(predictionsFile);
    std::ifstream truthFile(groundTruthFile);
    std::ofstream accFile(accuracyFile);

    if (!predFile.is_open() || !truthFile.is_open() || !accFile.is_open()) {
        std::cerr << "Error opening prediction, ground truth, or accuracy file." << std::endl;
        return;
    }

    std::string predLine, truthLine;
    int total = 0;
    int correct = 0;
    std::vector<std::string> errors;

    // Skip header lines if present
    auto skipHeader = [](std::ifstream& file) {
        if (!file.eof()) {
            std::streampos originalPos = file.tellg();
            std::string line;
            std::getline(file, line);
            std::string lowerLine = line;
            std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
            if (lowerLine.find("sentiment") != std::string::npos || lowerLine.find("id") != std::string::npos) {
                // Header detected, skip this line
            } else {
                // Reset to the beginning if no header
                file.clear();
                file.seekg(originalPos);
            }
        }
    };

    skipHeader(predFile);
    skipHeader(truthFile);

    while (std::getline(predFile, predLine) && std::getline(truthFile, truthLine)) {
        ++total;

        // Remove any trailing carriage return characters (for Windows compatibility)
        if (!predLine.empty() && predLine.back() == '\r') {
            predLine.pop_back();
        }
        if (!truthLine.empty() && truthLine.back() == '\r') {
            truthLine.pop_back();
        }

        // Parse prediction line
        std::stringstream ssPred(predLine);
        std::string predSentimentStr, predIDStr;
        std::getline(ssPred, predSentimentStr, ',');
        std::getline(ssPred, predIDStr);

        // Parse ground truth line
        std::stringstream ssTruth(truthLine);
        std::string truthSentimentStr, truthIDStr;
        std::getline(ssTruth, truthSentimentStr, ',');
        std::getline(ssTruth, truthIDStr);

        try {
            int predSentiment = std::stoi(predSentimentStr);
            int truthSentiment = std::stoi(truthSentimentStr);

            if (predSentiment == truthSentiment) {
                correct++;
            } else {
                errors.push_back(std::to_string(predSentiment) + ", " + truthSentimentStr + ", " + predIDStr);
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data in predictions or ground truth at line " << total << std::endl;
            continue; // Skip this line
        }
    }

    // Calculate accuracy
    double accuracy = (total > 0) ? static_cast<double>(correct) / total : 0.0;

    // Write accuracy and errors to file
    accFile << std::fixed << std::setprecision(3) << accuracy << "\n";
    for (const auto& errorLine : errors) {
        accFile << errorLine << "\n";
    }

    predFile.close();
    truthFile.close();
    accFile.close();

    auto end = std::chrono::steady_clock::now();  // End timer
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Evaluation time: " << elapsed_seconds.count() << "s\n";
}