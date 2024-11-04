#include "SentimentClassifier.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

// Tweet implementation
Tweet::Tweet(const DSString& id, const DSString& text, int sentiment)
    : id(id), text(text), sentiment(sentiment) {}

// Helper function to tokenize text into words
std::vector<DSString> SentimentClassifier::tokenize(const DSString& text) {
    std::vector<DSString> tokens;
    std::stringstream ss(text.c_str());
    std::string token;
    
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(DSString(token.c_str()));
    }
    
    return tokens;
}

// Preprocess word (lowercase, remove punctuation)
DSString SentimentClassifier::preprocessWord(const DSString& word) {
    std::string processed;
    std::string original = word.c_str();
    
    for (char c : original) {
        if (std::isalpha(c)) {
            processed += std::tolower(c);
        }
    }
    
    return DSString(processed.c_str());
}

// Update word frequency in the appropriate map
void SentimentClassifier::updateWordFrequency(const DSString& word, bool isPositive) {
    if (isPositive) {
        positiveWords[word]++;
    } else {
        negativeWords[word]++;
    }
}

// Train the classifier
void SentimentClassifier::train(const DSString& trainingFile) {
    std::ifstream file(trainingFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open training file");
    }
    
    std::string line;
    // Skip header if exists
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string sentiment, id, date, query, user, text;
        
        // Parse CSV line
        std::getline(ss, sentiment, ',');
        std::getline(ss, id, ',');
        std::getline(ss, date, ',');
        std::getline(ss, query, ',');
        std::getline(ss, user, ',');
        std::getline(ss, text);
        
        // Remove quotes if present
        if (text.front() == '"') text = text.substr(1);
        if (text.back() == '"') text = text.substr(0, text.length() - 1);
        
        bool isPositive = (sentiment == "4");
        
        // Tokenize and process each word
        auto tokens = tokenize(DSString(text.c_str()));
        for (const auto& token : tokens) {
            DSString processedWord = preprocessWord(token);
            if (processedWord.getLength() > 0) {
                updateWordFrequency(processedWord, isPositive);
            }
        }
    }
}

// Predict sentiment for a single tweet
int SentimentClassifier::predictSentiment(const DSString& text) {
    int positiveScore = 0;
    int negativeScore = 0;
    
    auto tokens = tokenize(text);
    for (const auto& token : tokens) {
        DSString processedWord = preprocessWord(token);
        if (processedWord.getLength() > 0) {
            positiveScore += positiveWords[processedWord];
            negativeScore += negativeWords[processedWord];
        }
    }
    
    return (positiveScore >= negativeScore) ? 4 : 0;
}

// Predict sentiments for test data
void SentimentClassifier::predict(const DSString& testFile, const DSString& predictionsFile) {
    std::ifstream inFile(testFile.c_str());
    std::ofstream outFile(predictionsFile.c_str());
    
    if (!inFile.is_open() || !outFile.is_open()) {
        throw std::runtime_error("Unable to open test file or predictions file");
    }
    
    std::string line;
    // Skip header
    std::getline(inFile, line);
    
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string id, date, query, user, text;
        
        // Parse CSV line (note: id is first in test data)
        std::getline(ss, id, ',');
        std::getline(ss, date, ',');
        std::getline(ss, query, ',');
        std::getline(ss, user, ',');
        std::getline(ss, text);
        
        // Remove quotes if present
        if (text.front() == '"') text = text.substr(1);
        if (text.back() == '"') text = text.substr(0, text.length() - 1);
        
        int sentiment = predictSentiment(DSString(text.c_str()));
        outFile << sentiment << "," << id << std::endl;
    }
}

// Evaluate predictions against ground truth
void SentimentClassifier::evaluatePredictions(const DSString& groundTruthFile,
                                            const DSString& predictionsFile,
                                            const DSString& accuracyFile) {
    std::ifstream truthFile(groundTruthFile.c_str());
    std::ifstream predFile(predictionsFile.c_str());
    std::ofstream accFile(accuracyFile.c_str());
    
    if (!truthFile.is_open() || !predFile.is_open() || !accFile.is_open()) {
        throw std::runtime_error("Unable to open files for evaluation");
    }
    
    // Skip header in truth file
    std::string header;
    std::getline(truthFile, header);
    
    int correct = 0;
    int total = 0;
    std::vector<std::pair<DSString, std::pair<int, int>>> errors; // id, (predicted, actual)
    
    std::string truthLine, predLine;
    while (std::getline(truthFile, truthLine) && std::getline(predFile, predLine)) {
        try {
            std::stringstream truthSS(truthLine);
            std::stringstream predSS(predLine);
            
            std::string truthSentiment, truthId, predSentiment, predId;
            
            std::getline(truthSS, truthSentiment, ',');
            std::getline(truthSS, truthId);
            std::getline(predSS, predSentiment, ',');
            std::getline(predSS, predId);
            
            // Debug output
            std::cout << "Processing:" << std::endl;
            std::cout << "Truth line: " << truthLine << std::endl;
            std::cout << "Pred line: " << predLine << std::endl;
            
            // Trim whitespace
            truthSentiment.erase(0, truthSentiment.find_first_not_of(" \t\r\n"));
            truthSentiment.erase(truthSentiment.find_last_not_of(" \t\r\n") + 1);
            predSentiment.erase(0, predSentiment.find_first_not_of(" \t\r\n"));
            predSentiment.erase(predSentiment.find_last_not_of(" \t\r\n") + 1);
            
            if (truthSentiment == predSentiment) {
                correct++;
            } else {
                errors.push_back({DSString(truthId.c_str()),
                                {std::stoi(predSentiment),
                                 std::stoi(truthSentiment)}});
            }
            total++;
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing line: " << e.what() << std::endl;
            std::cerr << "Truth line: " << truthLine << std::endl;
            std::cerr << "Pred line: " << predLine << std::endl;
            throw;
        }
    }
    
    if (total == 0) {
        throw std::runtime_error("No valid predictions processed");
    }
    
    // Write accuracy
    double accuracy = static_cast<double>(correct) / total;
    accFile << std::fixed << std::setprecision(3) << accuracy << std::endl;
    
    // Write errors
    for (const auto& error : errors) {
        accFile << error.second.first << "," << error.second.second << "," 
                << error.first << std::endl;
    }
} 