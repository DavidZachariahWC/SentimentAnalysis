#ifndef SENTIMENT_CLASSIFIER_H
#define SENTIMENT_CLASSIFIER_H

#include "DSString.h"
#include <vector>
#include <unordered_map>

class Tweet {
private:
    DSString id;
    DSString text;
    int sentiment;  // 0 for negative, 4 for positive

public:
    Tweet(const DSString& id, const DSString& text, int sentiment = -1);
    const DSString& getId() const { return id; }
    const DSString& getText() const { return text; }
    int getSentiment() const { return sentiment; }
    void setSentiment(int s) { sentiment = s; }
};

class SentimentClassifier {
private:
    // Word frequency maps for positive and negative sentiments
    std::unordered_map<DSString, int> positiveWords;
    std::unordered_map<DSString, int> negativeWords;
    
    // Helper functions
    std::vector<DSString> tokenize(const DSString& text);
    void updateWordFrequency(const DSString& word, bool isPositive);
    int predictSentiment(const DSString& text);
    DSString preprocessWord(const DSString& word);

public:
    SentimentClassifier() = default;
    
    // Core functionality
    void train(const DSString& trainingFile);
    void predict(const DSString& testFile, const DSString& predictionsFile);
    void evaluatePredictions(const DSString& groundTruthFile, 
                           const DSString& predictionsFile,
                           const DSString& accuracyFile);
};

#endif 