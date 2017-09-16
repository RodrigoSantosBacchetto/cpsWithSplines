
#include "../experiments/bigExperimentForTresholdAndErrorRate.hpp"

int main(int argc, char *argv[]){

    //define experiment parameters here
    std::string imageDbBasePath = "D:\\FP-UNA\\TesisFinalExperiments\\ImageDatabases\\MPEG7_SeparationExperiment\\";
    std::string resultsBasePath = "D:\\FP-UNA\\TesisFinalExperiments\\ExperimentResults\\";
    int sampleSize = atoi(argv[1]);
    double noiseLevel = 25.0;

    Coefficients = initializeEquationSolver(sampleSize);

    // run the experiment, based on the imported header file
    int rs = runExperiment(imageDbBasePath, resultsBasePath, sampleSize, noiseLevel);
    std::cout << rs;

}









