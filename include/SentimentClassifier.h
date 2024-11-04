#ifndef SENTIMENT_CLASSIFIER_H
#define SENTIMENT_CLASSIFIER_H

#include "DSString.h"
#include <vector>
#include <unordered_map>

// Represents a tweet with its metadata and sentiment (0=negative, 4=positive)
class Tweet {
private:
    DSString id;
    DSString text;
    int sentiment;

public:
    Tweet(const DSString& id, const DSString& text, int sentiment = -1);
    const DSString& getId() const { return id; }
    const DSString& getText() const { return text; }
    int getSentiment() const { return sentiment; }
    void setSentiment(int s) { sentiment = s; }
};

// Analyzes tweet sentiment using word frequency analysis
// Training: O(N * W), Prediction: O(W), Space: O(V)
// where N = tweets, W = words per tweet, V = vocabulary size
class SentimentClassifier {
private:
    // Word frequency maps for sentiment analysis
    std::unordered_map<DSString, int> positiveWords;
    std::unordered_map<DSString, int> negativeWords;
    
    // Core text processing functions
    std::vector<DSString> tokenize(const DSString& text);
    void updateWordFrequency(const DSString& word, bool isPositive);
    int predictSentiment(const DSString& text);
    DSString preprocessWord(const DSString& word);

public:
    SentimentClassifier() = default;
    
    // Main classifier operations
    void train(const DSString& trainingFile);
    void predict(const DSString& testFile, const DSString& predictionsFile);
    void evaluatePredictions(const DSString& groundTruthFile, 
                           const DSString& predictionsFile,
                           const DSString& accuracyFile);
};

#endif 