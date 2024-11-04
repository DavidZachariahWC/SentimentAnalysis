#include "SentimentClassifier.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

// Tweet implementation
Tweet::Tweet(const DSString& id, const DSString& text, int sentiment)
    : id(id), text(text), sentiment(sentiment) {}

// Enhanced tokenization that preserves emoticons and handles punctuation better
std::vector<DSString> SentimentClassifier::tokenize(const DSString& text) {
    std::vector<DSString> tokens;
    std::string str = text.c_str();
    std::string current;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        // Handle emoticons specially
        if (c == ':' || c == '=' || c == ';') {
            if (i + 1 < str.length()) {
                char next = str[i + 1];
                if (next == ')' || next == '(' || next == 'D' || next == 'P') {
                    if (!current.empty()) {
                        tokens.push_back(DSString(current.c_str()));
                        current.clear();
                    }
                    tokens.push_back(DSString(str.substr(i, 2).c_str()));
                    i++;
                    continue;
                }
            }
        }
        
        // Handle special characters that might indicate sentiment
        if (c == '!' || c == '?' || c == '.') {
            if (!current.empty()) {
                tokens.push_back(DSString(current.c_str()));
                current.clear();
            }
            // Count multiple punctuation marks
            std::string punct;
            while (i < str.length() && (str[i] == '!' || str[i] == '?' || str[i] == '.')) {
                punct += str[i];
                i++;
            }
            i--;
            if (punct.length() > 1) {  // Multiple punctuation might indicate strong sentiment
                tokens.push_back(DSString(punct.c_str()));
            }
            continue;
        }
        
        if (c == ' ' || c == ',' || c == '\t' || c == '\n') {
            if (!current.empty()) {
                tokens.push_back(DSString(current.c_str()));
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(DSString(current.c_str()));
    }
    
    return tokens;
}

// Enhanced stop words list with sentiment-aware filtering
bool isStopWord(const std::string& word) {
    static const std::unordered_set<std::string> stopWords = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have",
        "i", "it", "for", "on", "with", "he", "as", "you",
        "do", "at", "this", "but", "his", "by", "from", "they",
        "we", "say", "her", "she", "or", "an", "will", "my",
        "all", "would", "there", "their", "what", "so", "up", "out",
        "if", "about", "who", "get", "which", "go", "me",
        "when", "make", "can", "like", "time", "just", "him",
        "take", "people", "into", "year", "your", "some"
    };
    
    // Don't filter out negative words as they're important for sentiment
    static const std::unordered_set<std::string> keepWords = {
        "not", "no", "never", "none", "nothing", "nowhere", "neither",
        "good", "bad", "great", "terrible", "awesome", "horrible"
    };
    
    return stopWords.find(word) != stopWords.end() && keepWords.find(word) == keepWords.end();
}

// Enhanced word preprocessing
DSString SentimentClassifier::preprocessWord(const DSString& word) {
    std::string processed;
    std::string original = word.c_str();
    
    // Preserve emoticons and multiple punctuation
    if (original.length() <= 3 && 
        (original.find(':') != std::string::npos || 
         original.find('=') != std::string::npos ||
         original.find('!') != std::string::npos)) {
        return word;
    }
    
    // Convert to lowercase and handle punctuation
    for (char c : original) {
        if (std::isalpha(c)) {
            processed += std::tolower(c);
        }
    }
    
    // Skip stop words unless they're important for sentiment
    if (processed.length() > 0 && !isStopWord(processed)) {
        return DSString(processed.c_str());
    }
    
    return DSString("");
}

// Enhanced word frequency update with context awareness
void SentimentClassifier::updateWordFrequency(const DSString& word, bool isPositive) {
    int weight = 1;
    std::string w = word.c_str();
    
    // Enhanced emoticon detection
    if (w == ":)" || w == ":-)" || w == ":D" || w == "=)" || w == "<3" || 
        w == ";)" || w == ";D" || w == ":-D" || w == "=D") {
        weight = 4;
        isPositive = true;
    }
    else if (w == ":(" || w == ":-(" || w == "=(" || w == ":'(" || 
             w == ":/" || w == ":-/" || w == ":|" || w == ":-|") {
        weight = 4;
        isPositive = false;
    }
    // Strong positive sentiment words
    else if (w == "love" || w == "awesome" || w == "excellent" || w == "amazing" ||
             w == "fantastic" || w == "perfect" || w == "wonderful" || w == "great") {
        weight = 3;
    }
    // Strong negative sentiment words
    else if (w == "hate" || w == "terrible" || w == "awful" || w == "horrible" ||
             w == "worst" || w == "sucks" || w == "disappointed" || w == "bad") {
        weight = 3;
    }
    // Multiple punctuation indicates strong sentiment
    else if (w.find("!!!") != std::string::npos || w.find("???") != std::string::npos) {
        weight = 2;
    }
    
    if (isPositive) {
        positiveWords[word] += weight;
    } else {
        negativeWords[word] += weight;
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

// Enhanced sentiment prediction
int SentimentClassifier::predictSentiment(const DSString& text) {
    double positiveScore = 0;
    double negativeScore = 0;
    int totalWords = 0;
    bool hasNegation = false;
    bool hasStrongPositive = false;
    bool hasStrongNegative = false;
    
    auto tokens = tokenize(text);
    
    // First pass: check for strong indicators
    for (const auto& token : tokens) {
        std::string word = token.c_str();
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        // Very strong positive indicators (reduced list to most reliable ones)
        if (word == "love" || word == "awesome" || word == "amazing" ||
            word == "thank" || word == "thanks" || word == "best") {
            hasStrongPositive = true;
            positiveScore += 0.8;  // Increased direct boost
        }
        
        // Very strong negative indicators (reduced list to most reliable ones)
        if (word == "hate" || word == "terrible" || word == "worst" ||
            word == "sucks" || word == "horrible") {
            hasStrongNegative = true;
            negativeScore += 0.8;  // Increased direct boost
        }
    }
    
    // Second pass: detailed analysis
    for (size_t i = 0; i < tokens.size(); i++) {
        DSString processedWord = preprocessWord(tokens[i]);
        std::string word = tokens[i].c_str();
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        // Core negation words only
        if (word == "not" || word == "no" || word == "never" || 
            word == "don't" || word == "doesn't" || word == "didn't") {
            hasNegation = true;
            continue;
        }
        
        if (processedWord.getLength() > 0) {
            // Calculate word weights with context
            double posWeight = static_cast<double>(positiveWords[processedWord]) / 
                             (positiveWords[processedWord] + negativeWords[processedWord] + 1);
            double negWeight = static_cast<double>(negativeWords[processedWord]) / 
                             (positiveWords[processedWord] + negativeWords[processedWord] + 1);
            
            // Apply negation
            if (hasNegation) {
                std::swap(posWeight, negWeight);
                hasNegation = false;
            }
            
            // Only count words with clear sentiment
            if (std::abs(posWeight - negWeight) > 0.2) {  // Increased threshold for more confidence
                positiveScore += posWeight;
                negativeScore += negWeight;
                totalWords++;
            }
        }
    }
    
    // Normalize scores
    if (totalWords > 0) {
        positiveScore /= totalWords;
        negativeScore /= totalWords;
    }
    
    // Apply strong sentiment multipliers
    if (hasStrongPositive && !hasNegation) {
        positiveScore *= 1.5;
    }
    if (hasStrongNegative && !hasNegation) {
        negativeScore *= 1.5;
    }
    
    // Check for emoticons (reduced to most reliable ones)
    for (const auto& token : tokens) {
        std::string w = token.c_str();
        if (w.find(":)") != std::string::npos || w.find(":D") != std::string::npos) {
            positiveScore += 0.5;
        }
        if (w.find(":(") != std::string::npos) {
            negativeScore += 0.5;
        }
    }
    
    // Final decision with higher confidence threshold
    const double CONFIDENCE_THRESHOLD = 0.15;  // Increased threshold
    double scoreDiff = positiveScore - negativeScore;
    
    if (std::abs(scoreDiff) < CONFIDENCE_THRESHOLD) {
        // For borderline cases, rely on strong indicators
        if (hasStrongPositive) return 4;
        if (hasStrongNegative) return 0;
        
        // Default to the more likely case based on training data
        return (positiveScore >= negativeScore) ? 4 : 0;
    }
    
    return (scoreDiff > 0) ? 4 : 0;
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