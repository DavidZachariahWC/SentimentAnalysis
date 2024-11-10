#include "SentimentClassifier.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

// Constructor to initialize a Tweet object with id, text, and sentiment
Tweet::Tweet(const DSString& id, const DSString& text, int sentiment)
    : id(id), text(text), sentiment(sentiment) {}

// Tokenizes the text while handling emoticons and punctuation effectively
std::vector<DSString> SentimentClassifier::tokenize(const DSString& text) {
    std::vector<DSString> tokens;
    std::string str = text.c_str();
    std::string current;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        // Handle emoticons by looking for common emoticon patterns
        if (c == ':' || c == '=' || c == ';') {
            if (i + 1 < str.length()) {
                char next = str[i + 1];
                if (next == ')' || next == '(' || next == 'D' || next == 'P') {
                    if (!current.empty()) {
                        tokens.push_back(DSString(current.c_str()));
