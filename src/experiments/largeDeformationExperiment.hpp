//
// Created by Santos on 11/27/2016.
//


#ifndef CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
#define CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H

#include "../main/main.hpp"
#include "../main/filesManagementFunctions.hpp"
#include "../main/drawUtilityClasses.hpp"
#include "../main/cpsFunctions.hpp"
#include "../main/contourUtilities.hpp"
#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories);
void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories);



void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    auto t1 = Clock::now(), t2 = Clock::now();
    //Explore class folders
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name;

        for(int j = 0; j < currentResult.images.size(); j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 16);
                t1 = Clock::now();
                MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_16 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 32);
                t1 = Clock::now();
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_32 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 64);
                t1 = Clock::now();
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_64 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 86);
                t1 = Clock::now();
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_86 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 128);
                t1 = Clock::now();
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_128 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 256);
                t1 = Clock::now();
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_256 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();


            }
        }
        cvWaitKey( 0 );
        resultsByClass.push_back(currentResult);
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\cps_plus_splines.csv", std::ios_base::out);

    outputFile << "CLASS\t16_HIT_MAX_DISTANCE_SAME\t32_HIT_MAX_DISTANCE_SAME\t64_HIT_MAX_DISTANCE_SAME\t16_HIT_MAX_DISTANCE_DIFF\t32_HIT_MAX_DISTANCE_DIFF\t64_HIT_MAX_DISTANCE_DIFF" << std::endl;

    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */

        outputFile << resultsByClass[i].execution_time_16 << "\t";


        outputFile << resultsByClass[i].execution_time_32 << "\t";


        outputFile << resultsByClass[i].execution_time_64 << "\t";

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */

        outputFile << resultsByClass[i].execution_time_86 << "\t";


        outputFile << resultsByClass[i].execution_time_128 << "\t";


        outputFile << resultsByClass[i].execution_time_256 << std::endl;

    }

    outputFile.close();
}


void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    auto t1 = Clock::now(), t2 = Clock::now();
    //Explore class folders
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name << std::endl;

        for(int j = 0; j < currentResult.images.size(); j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));

                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 16);
                t1 = Clock::now();
                MatrixXd cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_16 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 32);
                t1 = Clock::now();
                cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_32 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 64);
                t1 = Clock::now();
                cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_64 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 86);
                t1 = Clock::now();
                cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_86 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 128);
                t1 = Clock::now();
                cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_128 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
                sampledPoints = sampleContourPoints(fullContour, 256);
                t1 = Clock::now();
                cpsMatrix = computeCps(sampledPoints, area);
                t2 = Clock::now();
                currentResult.execution_time_256 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            }

        }
        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\normal_cps.csv", std::ios_base::out);

    outputFile << "CLASS\t32_HIT_MAX_DISTANCE\t64_HIT_MAX_DISTANCE\t128_HIT_MAX_DISTANCE" << std::endl;

    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        outputFile << resultsByClass[i].execution_time_16 << "\t";


        outputFile << resultsByClass[i].execution_time_32 << "\t";


        outputFile << resultsByClass[i].execution_time_64 << "\t";

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */

        outputFile << resultsByClass[i].execution_time_86 << "\t";


        outputFile << resultsByClass[i].execution_time_128 << "\t";


        outputFile << resultsByClass[i].execution_time_256 << std::endl;

    }

    outputFile.close();
}
#endif //CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
