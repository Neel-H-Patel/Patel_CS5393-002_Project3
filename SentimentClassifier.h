#ifndef SENTIMENTCLASSIFIER_H
#define SENTIMENTCLASSIFIER_H

#include <unordered_map>
#include "DSString.h"

class SentimentClassifier {
private:
    // Word counts for positive and negative sentiments
    std::unordered_map<DSString, int> positiveWordCounts;
    std::unordered_map<DSString, int> negativeWordCounts;

    // Total number of positive and negative tweets
    int totalPositiveTweets = 0;
    int totalNegativeTweets = 0;

public:
    void train(const char* trainingDataFile);
    void predict(const char* testingDataFile, const char* outputFile);
    void evaluatePredictions(const char* predictionsFile, const char* groundTruthFile, const char* accuracyFile);
};

#endif
