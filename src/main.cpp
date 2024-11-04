#include "SentimentClassifier.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <training_file> <test_file> <test_sentiment_file> "
                  << "<predictions_file> <accuracy_file>" << std::endl;
        return 1;
    }
    
    try {
        SentimentClassifier classifier;
        
        // Train the classifier
        std::cout << "Training classifier..." << std::endl;
        classifier.train(argv[1]);
        
        // Make predictions
        std::cout << "Making predictions..." << std::endl;
        classifier.predict(argv[2], argv[4]);
        
        // Evaluate results
        std::cout << "Evaluating results..." << std::endl;
        classifier.evaluatePredictions(argv[3], argv[4], argv[5]);
        
        std::cout << "Classification complete! Check " << argv[5] << " for results." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 