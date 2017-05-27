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

void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories);
void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories);



void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    //Explore class folders
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name;

        for(int j = 0; j < (currentResult.images.size()-1); j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 86);
                MatrixXd cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_32.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 128);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_64.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 256);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_128.push_back(cpsMatrix);
            }

            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j+1], ONLY_EXTERNAL_CONTOUR);

            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 86);
            MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 256);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrix)[1]);
        }
        cvWaitKey( 0 );
        resultsByClass.push_back(currentResult);

        std::cout << std::endl << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\cps_plus_splines.csv", std::ios_base::out);

    outputFile << "CLASS\t32_HIT_MAX_DISTANCE\t64_HIT_MAX_DISTANCE\t128_HIT_MAX_DISTANCE" << std::endl;

    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */
        double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");

        outputFile << valueSame32 << "\t";

        double valueSame64 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");

        outputFile << valueSame64 << "\t";

        double valueSame128 = getMaxMinValue(resultsByClass[i].same_class_distances_128 , "max");

        outputFile << valueSame128 << std::endl;

    }

    outputFile.close();
}


void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    //Explore class folders
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name << std::endl;

        for(int j = 0; j < (currentResult.images.size()-1); j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 86);
                MatrixXd cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_32.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 128);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_64.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 256);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_128.push_back(cpsMatrix);
            }
            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j + 1], ONLY_EXTERNAL_CONTOUR);
            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 86);
            MatrixXd cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 256);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrix)[1]);

        }

        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\normal_cps.csv", std::ios_base::out);

    outputFile << "CLASS\t32_HIT_MAX_DISTANCE\t64_HIT_MAX_DISTANCE\t128_HIT_MAX_DISTANCE" << std::endl;

    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */
        double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");

        outputFile << valueSame32 << "\t";

        double valueSame64 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");

        outputFile << valueSame64 << "\t";

        double valueSame128 = getMaxMinValue(resultsByClass[i].same_class_distances_128 , "max");

        outputFile << valueSame128 << std::endl;

    }

    outputFile.close();
}
#endif //CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
