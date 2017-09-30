
#include "../experiments/bullsEyeExperiment.hpp"

int main(int argc, char *argv[]){

    std::cout << "Initializing Experiment:\n";
    //define experiment parameters here
    std::string imageDbBasePath(argv[1]);
    std::cout << " - Image Database Path: " << imageDbBasePath << "\n";
    std::string resultsBasePath(argv[2]);
    std::cout << " - Results Folder: " << resultsBasePath << "\n";
    int sampleSize = atoi(argv[3]);
    std::cout << " - Sample Size: " << sampleSize << "\n";
    double noiseLevel = atof(argv[4]);
    std::cout << " - Noise (dB): " << ((noiseLevel < 0.0) ? "Not using noise." : argv[4]) << "\n\n";

    Coefficients = initializeEquationSolver(sampleSize);

    // run the experiment, based on the imported header file
    int rs = runExperiment(imageDbBasePath, resultsBasePath, "Refined", sampleSize, noiseLevel);
    std::cout << rs;

}









