//
// Created by Gerardo on 07/08/2017.
//

#ifndef CPSWITHSPLINES_BULL_H
#define CPSWITHSPLINES_BULL_H

#include "../main/cpsFunctions.hpp"
#include "../main/filesManagementFunctions.hpp"
#include "../main/noiseFunctions.hpp"

#define EXPERIMENT_NAME "bullsEye"

typedef struct distance_class {
    std::string imageClass;
    int imageId;
    double distance;

    bool operator<(const distance_class& rhs) const {
        return distance < rhs.distance;
    }
} distanceClass;

typedef struct results_tracker {
    std::string imageClass;
    CPSResult shapeDescriptor;
    int imageId;
    std::vector<distanceClass> bestDistances;
} resultsTracker;

std::string getCurrentTimeAsString(void);
std::map< int, std::string > imageClassName;
void initializeImageClassName();
std::string completeZeroes(int);
std::string noiseTag(double);
void addNewDistance(std::vector<distanceClass> &currentDistanceArray, distanceClass newDistance);

void printResults(std::vector<resultsTracker> allResultsSoFar ) {
    std::cout << "     Results so far...\n";
    for (int k = 0; k < allResultsSoFar.size(); k++) {
        std::cout << "         " << allResultsSoFar[k].imageId << " - " << allResultsSoFar[k].imageClass << "\n";
        if (allResultsSoFar[k].bestDistances.size() == 0) {
            std::cout << "            * Nothing to show yet!\n";
        } else {
            for (int p = 0; p < allResultsSoFar[k].bestDistances.size(); p++) {
                std::cout << "            * "
                << allResultsSoFar[k].bestDistances[p].imageId
                << " (" << allResultsSoFar[k].bestDistances[p].imageClass
                << ") - " << allResultsSoFar[k].bestDistances[p].distance  << "\n";
            }
        }
    }
    std::cin.ignore();
    std::cin.get();
}

int runExperiment(std::string imgDbBasePath, std::string resultsFolderBasePath, std::string cpsType, int sampleSize, double noiseLevel) {

    initializeImageClassName();

    std::cout << "Reading image directories...";
    std::vector<std::string> classDirectories = getClassDirectories(imgDbBasePath);
    std::vector<std::string> imageDirectories;
    for (int i = 0; i < classDirectories.size(); i++) {
        std::vector<std::string> imagesOnThisClass = getImageDirectories(classDirectories[i]);
        imageDirectories.insert(imageDirectories.end(), imagesOnThisClass.begin(), imagesOnThisClass.end() );
    }
    std::cout << " [DONE]\n";

    // some aux variables, global to the process
    std::vector<resultsTracker> allImageResults;

    // Start experiment
    std::string startingTimestamp = getCurrentTimeAsString();
    std::cout << "Started experiment: " << startingTimestamp << "\n";


    std::cout << "Reading images and processing:\n";
    std::string lastClass = "NoNo";
    //std::stringstream R;
    for (int imagePtr = 0; imagePtr < imageDirectories.size(); imagePtr++) {

        //R.str("");

        // Read the image
        gImage thisImage = readImageOnPath(imageDirectories[imagePtr]);
        if (thisImage.imageClass.compare(lastClass) != 0) {
            lastClass = thisImage.imageClass;
            std::cout << " >> " << lastClass << "\n";
        }
        //R << "     " << imagePtr << " - " << thisImage.imageName << " (" << thisImage.imageClass << ")\n";

        // Initialize results tracker
        resultsTracker resultsForThisImage;
        resultsForThisImage.imageId = imagePtr;
        resultsForThisImage.imageClass = thisImage.imageClass;

        // Calculate the CPS
        std::vector<cv::Point> imageContour = getKuimContour(thisImage.image, ONLY_EXTERNAL_CONTOUR);
        std::vector<cv::Point> workContour;

        if (noiseLevel < 0.0) {
            workContour = imageContour;
        } else {
            workContour = getNoisyContour(imageContour, noiseLevel);
        }

        if (cpsType.compare("Regular") == 0) {
            resultsForThisImage.shapeDescriptor = generateCpsFromContourAndSampleSize(workContour, sampleSize);
        } else {
            resultsForThisImage.shapeDescriptor = generateSplineCpsFromContourAndSampleSize(workContour, sampleSize);
        }

        // Process the results for this image against all the others
        if ( imagePtr != 0 ) {
            for (int i = 0; i < imagePtr; i++) {
                double disimilarity = similarityMeasure(
                        resultsForThisImage.shapeDescriptor,
                        allImageResults[i].shapeDescriptor,
                        0.5,
                        0.5);

                //R << "         " << i << " (" << allImageResults[i].imageClass << ") - " << disimilarity << "\n";

                distanceClass distanceObjectForOtherImage;
                distanceObjectForOtherImage.imageId = imagePtr;
                distanceObjectForOtherImage.imageClass = thisImage.imageClass;
                distanceObjectForOtherImage.distance = disimilarity;

                distanceClass distanceObjectForThisImage;
                distanceObjectForThisImage.imageId = i;
                distanceObjectForThisImage.imageClass = allImageResults[i].imageClass;
                distanceObjectForThisImage.distance = disimilarity;

                addNewDistance(allImageResults[i].bestDistances, distanceObjectForOtherImage);
                addNewDistance(resultsForThisImage.bestDistances, distanceObjectForThisImage);
            }
        }
        //std::cout << R.str();
        allImageResults.push_back(resultsForThisImage);
        //printResults(allImageResults);
    }

    //prepare file with results
    std::stringstream filename, fileEntry;
    filename << resultsFolderBasePath << EXPERIMENT_NAME << "\\" << cpsType << "_" << completeZeroes(sampleSize)
             << "_" << noiseTag(noiseLevel) << ".txt";
    std::fstream resultsFile;
    resultsFile.open(filename.str().c_str(), std::ios::out | std::ios::app );
    // Process our results and compute Bull's Eye measure
    double k = 0;
    for ( int i = 0; i < allImageResults.size(); i++ ) {
        std::cout << allImageResults[i].imageId << ": " << allImageResults[i].bestDistances.size() << " - ";
        fileEntry.str("");
        fileEntry << allImageResults[i].imageId << ": ";
        int n = 0;
        for ( int j = 0; j < allImageResults[i].bestDistances.size(); j++ ) {
            if ( allImageResults[i].imageClass.compare(allImageResults[i].bestDistances[j].imageClass) == 0 ) {
                k++;
                n++;
            }
        }
        std::cout << n << "\n";
        fileEntry << n << "/" << allImageResults[i].bestDistances.size() << "\n";
        resultsFile << fileEntry.str();
    }
    fileEntry.str("");
    fileEntry << "\nBull's Eye Measure: " << k/(20*1400);
    resultsFile << fileEntry.str();
    resultsFile.close();
    std::cout << "Bull's Eye measure: " << k/(20*1400);

    return 0;
}

void addNewDistance(std::vector<distanceClass> &distanceArray, distanceClass newDistance) {

    if ( distanceArray.size() < 40 ) {
        distanceArray.push_back(newDistance);
    } else {
        if ( newDistance < distanceArray[39] ) {
            distanceArray.pop_back();
            distanceArray.push_back(newDistance);
        }
    }

    std::sort(distanceArray.begin(), distanceArray.end());
}

std::string getCurrentTimeAsString() {
    time_t t = time(0);   // get time now
    struct tm *now = localtime(&t);

    std::stringstream ss;
    ss << (now->tm_year + 1900)
    << "-" << (now->tm_mon + 1 < 10 ? "0" : "")
    << (now->tm_mon + 1)
    << "-" << (now->tm_mday + 1 < 10 ? "0" : "")
    << now->tm_mday
    << " " << (now->tm_hour + 1 < 10 ? "0" : "")
    << now->tm_hour
    << ":" << (now->tm_min + 1 < 10 ? "0" : "")
    << now->tm_min
    << ":" << (now->tm_sec + 1 < 10 ? "0" : "")
    << now->tm_sec
    << std::endl;

    return ss.str();
}

void initializeImageClassName () {
    imageClassName.insert({0, "apple"});
    imageClassName.insert({1, "bat"});
    imageClassName.insert({2, "beetle"});
    imageClassName.insert({3, "bell"});
    imageClassName.insert({4, "bird"});
    imageClassName.insert({5, "Bone"});
    imageClassName.insert({6, "bottle"});
    imageClassName.insert({7, "brick"});
    imageClassName.insert({8, "butterfly"});
    imageClassName.insert({9, "camel"});
    imageClassName.insert({10, "car"});
    imageClassName.insert({11, "carriage"});
    imageClassName.insert({12, "cattle"});
    imageClassName.insert({13, "cellular_phone"});
    imageClassName.insert({14, "chicken"});
    imageClassName.insert({15, "children"});
    imageClassName.insert({16, "chopper"});
    imageClassName.insert({17, "classic"});
    imageClassName.insert({18, "Comma"});
    imageClassName.insert({19, "crown"});
    imageClassName.insert({20, "cup"});
    imageClassName.insert({21, "deer"});
    imageClassName.insert({22, "device0"});
    imageClassName.insert({23, "device1"});
    imageClassName.insert({24, "device2"});
    imageClassName.insert({25, "device3"});
    imageClassName.insert({26, "device4"});
    imageClassName.insert({27, "device5"});
    imageClassName.insert({28, "device6"});
    imageClassName.insert({29, "device7"});
    imageClassName.insert({30, "device8"});
    imageClassName.insert({31, "device9"});
    imageClassName.insert({32, "dog"});
    imageClassName.insert({33, "elephant"});
    imageClassName.insert({34, "face"});
    imageClassName.insert({35, "fish"});
    imageClassName.insert({36, "flatfish"});
    imageClassName.insert({37, "fly"});
    imageClassName.insert({38, "fork"});
    imageClassName.insert({39, "fountain"});
    imageClassName.insert({40, "frog"});
    imageClassName.insert({41, "Glas"});
    imageClassName.insert({42, "guitar"});
    imageClassName.insert({43, "hammer"});
    imageClassName.insert({44, "hat"});
    imageClassName.insert({45, "HCircle"});
    imageClassName.insert({46, "Heart"});
    imageClassName.insert({47, "horse"});
    imageClassName.insert({48, "horseshoe"});
    imageClassName.insert({49, "jar"});
    imageClassName.insert({50, "key"});
    imageClassName.insert({51, "lizzard"});
    imageClassName.insert({52, "lmfish"});
    imageClassName.insert({53, "Misk"});
    imageClassName.insert({54, "octopus"});
    imageClassName.insert({55, "pencil"});
    imageClassName.insert({56, "personal_car"});
    imageClassName.insert({57, "pocket"});
    imageClassName.insert({58, "rat"});
    imageClassName.insert({59, "ray"});
    imageClassName.insert({60, "sea_snake"});
    imageClassName.insert({61, "shoe"});
    imageClassName.insert({62, "spoon"});
    imageClassName.insert({63, "spring"});
    imageClassName.insert({64, "stef"});
    imageClassName.insert({65, "teddy"});
    imageClassName.insert({66, "tree"});
    imageClassName.insert({67, "truck"});
    imageClassName.insert({68, "turtle"});
    imageClassName.insert({69, "watch"});
}

std::string completeZeroes(int N) {
    std::stringstream s;
    
    if (N < 10) { s << "00"; }
    else if (N < 100) { s << "0"; }
    
    s << N;
    return s.str();
}

std::string noiseTag(double N) {
    std::stringstream s;

    if (N < 0.0) { s << "NO-NOISE"; }
    else { s << (int)N << "-dB"; }
    
    return s.str();
}

#endif