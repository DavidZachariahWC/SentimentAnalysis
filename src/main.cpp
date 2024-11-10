#include "SentimentClassifier.h"
#include <iostream>

/**
 * @brief Entry function for sentiment analysis application
 * 
 * This function processes the command line arguments and executes the sentiment classifier's
 * tasks including training, making predictions, and assessing accuracy.
 * 
 * Expected command line arguments:
 * 1. Path to the training data file
 * 2. Path to the test data file
 * 3. Path to the file containing test sentiment labels
 * 4. Path where the prediction results will be saved
 * 5. Path where the accuracy results will be saved
 * 
 * @param argc The count of command line arguments
 * @param argv Array containing the command line arguments
 * @return 0 for successful execution, 1 for errors
 */
int main(int argc, char** argv) {
    // Check if the correct number of arguments are provided
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <training_file> <test_file> <test_sentiment_file> "
                  << "<predictions_file> <accuracy_file>" << std::endl;
        return 1;
    }
    
    try {
        SentimentClassifier classifier;
        
        // Train the sentiment classifier using the provided training data
        std::cout << "Training the classifier..." << std::endl;
        classifier.train(argv[1]);
        
        // Generate predictions using the test data
        std::cout << "Generating predictions..." << std::endl;
        classifier.predict(argv[2], argv[4]);
        
        // Assess the accuracy of the predictions
        std::cout << "Assessing the results..." << std::endl;
        classifier.evaluatePredictions(argv[3], argv[4], argv[5]);
        
        std::cout << "Sentiment classification completed! Results saved in " << argv[5] << "." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
