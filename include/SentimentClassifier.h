#ifndef SENTIMENT_CLASSIFIER_H
#define SENTIMENT_CLASSIFIER_H

#include "DSString.h"
#include <vector>
#include <unordered_map>

// A class representing a tweet, including its metadata and sentiment value (0 for negative, 4 for positive)
class Tweet {
private:
    DSString id;           // Unique identifier for the tweet
    DSString text;         // The text content of the tweet
    int sentiment;         // The sentiment score of the tweet (-1 for unknown, 0=negative, 4=positive)

public:
    // Constructor to initialize a tweet with id, text, and optional sentiment
    Tweet(const DSString& id, const DSString& text, int sentiment = -1);

    // Getter functions
    const DSString& getId() const { return id; }               // Return the tweet's ID
    const DSString& getText() const { return text; }           // Return the tweet's text
    int getSentiment() const { return sentiment; }             // Return the tweet's sentiment
    void setSentiment(int s) { sentiment = s; }                // Set the tweet's sentiment
};

// A class to analyze tweet sentiments using word frequency analysis
// Training Time Complexity: O(N * W), where N is the number of tweets and W is the average number of words per tweet
// Prediction Time Complexity: O(W), where W is the number of words in the tweet
// Space Complexity: O(V), where V is the size of the vocabulary
class SentimentClassifier {
private:
    // Maps for tracking the frequency of positive and negative words in the training data
    std::unordered_map<DSString, int> positiveWords;
    std::unordered_map<DSString, int> negativeWords;

    // Core text processing functions for sentiment analysis
    std::vector<DSString> tokenize(const DSString& text);           // Tokenizes the text into words
    void updateWordFrequency(const DSString& word, bool isPositive); // Updates word frequency for sentiment analysis
    int predictSentiment(const DSString& text);                     // Predicts the sentiment of a given text
    DSString preprocessWord(const DSString& word);                 // Preprocesses the word for consistency (e.g., case normalization)

public:
    // Default constructor
    SentimentClassifier() = default;

    // Main functions for training, prediction, and evaluation
    void train(const DSString& trainingFile);                        // Trains the classifier using a training file
    void predict(const DSString& testFile, const DSString& predictionsFile); // Predicts sentiment for the test data and stores predictions
    void evaluatePredictions(const DSString& groundTruthFile,      // Evaluates the classifier’s performance by comparing predicted vs ground truth sentiment
                            const DSString& predictionsFile,
                            const DSString& accuracyFile);         // Writes evaluation results (e.g., accuracy) to a file
};

#endif
