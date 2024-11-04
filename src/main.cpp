#include "SentimentClassifier.h"
#include <iostream>

/**
 * @brief Main entry point for sentiment analysis program
 * 
 * Processes command line arguments and runs the sentiment classifier
 * through its training, prediction, and evaluation phases.
 * 
 * Expected arguments:
 * 1. Training data file path
 * 2. Test data file path
 * 3. Test sentiment file path
 * 4. Predictions output file path
 * 5. Accuracy output file path
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, 1 on error
 */
int main(int argc, char** argv) {
    // Validate command line arguments
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <training_file> <test_file> <test_sentiment_file> "
                  << "<predictions_file> <accuracy_file>" << std::endl;
        return 1;
    }
    
    try {
        SentimentClassifier classifier;
        
        // Train the classifier on labeled data
        std::cout << "Training classifier..." << std::endl;
        classifier.train(argv[1]);
        
        // Make predictions on test data
        std::cout << "Making predictions..." << std::endl;
        classifier.predict(argv[2], argv[4]);
        
        // Evaluate prediction accuracy
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