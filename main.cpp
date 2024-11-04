#include <iostream>
#include "SentimentClassifier.h"

int main(int argc, char** argv)
{
    if (argc != 6) {
        std::cerr << "Usage: ./Assignment2_sentiment trainingDataFile testingDataFile groundTruthFile predictionsFile accuracyFile" << std::endl;
        return 1;
    }

    SentimentClassifier classifier;

    classifier.train(argv[1]);
    classifier.predict(argv[2], argv[4]);
    classifier.evaluatePredictions(argv[4], argv[3], argv[5]);

    return 0;
}
