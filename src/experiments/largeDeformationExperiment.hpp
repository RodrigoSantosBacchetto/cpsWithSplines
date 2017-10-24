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

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\cps_plus_splines.csv", std::ios_base::out);
    double alfa = 0.5;
        for (int i = 0; i < imageClassesDirectories.size(); i++) {
            classResults currentResult;

            //Store class name
            currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

            //Read all class images
            currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

            //Compute results for each image of the class
            std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name;

            for (int j = 0; j < (currentResult.images.size() - 1) / 2; j++) {
                /*If j=0 calc the first image cps data*/
                if (j == 0) {
                    std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);
                    std::cout << std::endl << "base image lenght [ " << fullContour.size() << " ]: " <<
                    std::endl;
                    /* Calculate the area for the contour in order to normalize*/
                    const double area = sqrt(contourArea(fullContour));
                    std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 8);
                    cspResult cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                    currentResult.cp_signatures_16.push_back(cpsMatrix);

                    sampledPoints = sampleContourPoints(fullContour, 16);
                    cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                    currentResult.cp_signatures_32.push_back(cpsMatrix);

                    sampledPoints = sampleContourPoints(fullContour, 32);
                    cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                    currentResult.cp_signatures_64.push_back(cpsMatrix);
                }

                /*Similar image*/
                std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j + 1],
                                                                           ONLY_EXTERNAL_CONTOUR);
                std::cout << std::endl << "similar image lenght [ " << fullContourSimilar.size() << " ]: " <<
                std::endl;
                /* Calculate the area for the contour in order to normalize*/
                const double areaSimilar = sqrt(contourArea(fullContourSimilar));

                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 8);
                cspResult cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
                currentResult.same_class_distances_16.push_back(
                        similarityMeasure(currentResult.cp_signatures_16[0], cpsMatrix, alfa, 1-alfa));

                sampledPoints = sampleContourPoints(fullContourSimilar, 16);
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
                currentResult.same_class_distances_32.push_back(
                        similarityMeasure(currentResult.cp_signatures_32[0], cpsMatrix, alfa, 1-alfa));

                sampledPoints = sampleContourPoints(fullContourSimilar, 32);
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
                currentResult.same_class_distances_64.push_back(
                        similarityMeasure(currentResult.cp_signatures_64[0], cpsMatrix, alfa, 1-alfa));

                /*different image*/
                std::vector<cv::Point> fullContourDifferent = getKuimContour(currentResult.images[j + 6],
                                                                             ONLY_EXTERNAL_CONTOUR);
                std::cout << std::endl << "different image lenght [ " << fullContourDifferent.size() << " ]: " <<
                std::endl;
                /* Calculate the area for the contour in order to normalize*/
                const double areaDifferent = sqrt(contourArea(fullContourDifferent));

                std::vector<cv::Point> sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 8);
                cspResult cpsMatrixDiff = generateCpsWithSplineRefinement(sampledPointsDifferent, areaDifferent);
                currentResult.diff_class_distances_16.push_back(
                        similarityMeasure(currentResult.cp_signatures_16[0], cpsMatrixDiff, alfa, 1-alfa));

                sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 16);
                cpsMatrixDiff = generateCpsWithSplineRefinement(sampledPointsDifferent, areaDifferent);
                currentResult.diff_class_distances_32.push_back(
                        similarityMeasure(currentResult.cp_signatures_32[0], cpsMatrixDiff, alfa, 1-alfa));

                sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 32);
                cpsMatrixDiff = generateCpsWithSplineRefinement(sampledPointsDifferent, areaDifferent);
                currentResult.diff_class_distances_64.push_back(
                        similarityMeasure(currentResult.cp_signatures_64[0], cpsMatrixDiff, alfa, 1-alfa));

            }
            cvWaitKey(0);
            resultsByClass.push_back(currentResult);

            std::cout << std::endl << "Finished processing class [ " << i << " ]: " << currentResult.class_name <<
            std::endl;
        }

        outputFile <<
        "CLASS\t16_HIT_MAX_DISTANCE_SAME\t32_HIT_MAX_DISTANCE_SAME\t64_HIT_MAX_DISTANCE_SAME\t16_HIT_MAX_DISTANCE_DIFF\t32_HIT_MAX_DISTANCE_DIFF\t64_HIT_MAX_DISTANCE_DIFF" <<
        std::endl;
        double maxValue = 0;
        for (int i = 0; i < resultsByClass.size(); i++) {

            outputFile << resultsByClass[i].class_name << "\t";

            /* Get the max value and min value for a vector of double */
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_16, "max");

            outputFile << valueSame32 << "\t";
            if(maxValue<valueSame32)
                maxValue = valueSame32;
            double valueSame64 = getMaxMinValue(resultsByClass[i].same_class_distances_32, "max");

            outputFile << valueSame64 << "\t";
            if(maxValue<valueSame64)
                maxValue = valueSame64;
            double valueSame128 = getMaxMinValue(resultsByClass[i].same_class_distances_64, "max");

            outputFile << valueSame128 << "\t";
            if(maxValue<valueSame128)
                maxValue = valueSame128;
            outputFile << resultsByClass[i].class_name << "\t";

            /* Get the max value and min value for a vector of double */
            valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_16, "min");

            outputFile << valueSame32 << "\t";
            if(maxValue<valueSame32)
                maxValue = valueSame32;

            valueSame64 = getMaxMinValue(resultsByClass[i].diff_class_distances_32, "min");

            outputFile << valueSame64 << "\t";
            if(maxValue<valueSame64)
                maxValue = valueSame64;
            valueSame128 = getMaxMinValue(resultsByClass[i].diff_class_distances_64, "min");

            outputFile << valueSame128 << std::endl;
            if(maxValue<valueSame128)
                maxValue = valueSame128;

        }
        double error = 100, umbralOptimo;
        for (double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
            double FP = 0, FN = 0;
            for (int i = 0; i < resultsByClass.size(); i++) {

                /* Get the max value and min value for a vector of double */
                double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_16, "max");
                if (valueSame32 > umbral) {
                    FP++;
                }


                valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_16, "min");
                if (valueSame32 < umbral) {
                    FN++;
                }
            }
            if (error > ((FP + FN) / (2 * imageClassesDirectories.size()))) {
                error = ((FP + FN) / (2 * imageClassesDirectories.size()));
                umbralOptimo = umbral;
            }
        }
        outputFile << "ERROR 16  = " << "\t";

        outputFile << error << "\t";
        outputFile << "UMBRAL OPTIMO = " << "\t";

        outputFile << umbralOptimo << "\t";

        error = 100;
        for (double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
            double FP = 0, FN = 0;
            for (int i = 0; i < resultsByClass.size(); i++) {

                /* Get the max value and min value for a vector of double */
                double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32, "max");
                if (valueSame32 > umbral) {
                    FP++;
                }

                /* Get the max value and min value for a vector of double */
                valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_32, "min");
                if (valueSame32 < umbral) {
                    FN++;
                }
            }
            if (error > ((FP + FN) / (2 * imageClassesDirectories.size()))) {
                error = ((FP + FN) / (2 * imageClassesDirectories.size()));
                umbralOptimo = umbral;
            }
        }
        outputFile << "ERROR 32  = " << "\t";
        outputFile << error << "\t";
        outputFile << "UMBRAL OPTIMO = " << "\t";
        outputFile << umbralOptimo << "\t";

        error = 100;
        for (double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
            double FP = 0, FN = 0;
            for (int i = 0; i < resultsByClass.size(); i++) {

                /* Get the max value and min value for a vector of double */
                double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_64, "max");
                if (valueSame32 > umbral) {
                    FP++;
                }

                /* Get the max value and min value for a vector of double */
                valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_64, "min");
                if (valueSame32 < umbral) {
                    FN++;
                }
            }
            if (error > ((FP + FN) / (2 * imageClassesDirectories.size()))) {
                error = ((FP + FN) / (2 * imageClassesDirectories.size()));
                umbralOptimo = umbral;
            }
        }
        outputFile << "ERROR 64  = " << "\t";
        outputFile << error << "\t";
        outputFile << "UMBRAL OPTIMO = " << "\t";
        outputFile << umbralOptimo << "\t";
        outputFile << "alfa y beta = " << "\t";
        outputFile << alfa << "-" << 1-alfa << "\t";
    outputFile.close();
}


void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    //Explore class folders
    double alfa = 0.5;
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name << std::endl;

        for(int j = 0; j < (currentResult.images.size()-1)/2; j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 32);
                cspResult cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_16.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 64);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_32.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 128);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_64.push_back(cpsMatrix);
            }

            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j+1], ONLY_EXTERNAL_CONTOUR);

            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar)) ;

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 32);
            cspResult cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_16.push_back(similarityMeasure(currentResult.cp_signatures_16[0],cpsMatrix,alfa,1-alfa));

            sampledPoints = sampleContourPoints(fullContourSimilar, 64);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_32.push_back(similarityMeasure(currentResult.cp_signatures_32[0],cpsMatrix,alfa,1-alfa));

            sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_64.push_back(similarityMeasure(currentResult.cp_signatures_64[0],cpsMatrix,alfa,1-alfa));

            /*different image*/
            std::vector<cv::Point> fullContourDifferent = getKuimContour(currentResult.images[j+6], ONLY_EXTERNAL_CONTOUR);

            /* Calculate the area for the contour in order to normalize*/
            const double areaDifferent = sqrt(contourArea(fullContourDifferent));

            std::vector<cv::Point> sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 32);
            cspResult cpsMatrixDiff = computeCps(sampledPointsDifferent, areaDifferent);
            currentResult.diff_class_distances_16.push_back(similarityMeasure(currentResult.cp_signatures_16[0],cpsMatrixDiff,alfa,1-alfa));

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 64);
            cpsMatrixDiff = computeCps(sampledPointsDifferent, areaDifferent);
            currentResult.diff_class_distances_32.push_back(similarityMeasure(currentResult.cp_signatures_32[0],cpsMatrixDiff,alfa,1-alfa));

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 128);
            cpsMatrixDiff = computeCps(sampledPointsDifferent, areaDifferent);
            currentResult.diff_class_distances_64.push_back(similarityMeasure(currentResult.cp_signatures_64[0],cpsMatrixDiff,alfa,1-alfa));

        }
        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\normal_cps.csv", std::ios_base::out);

    outputFile << "CLASS\t32_HIT_MAX_DISTANCE\t64_HIT_MAX_DISTANCE\t128_HIT_MAX_DISTANCE" << std::endl;
    double maxValue =0;
    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */
        double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_16 , "max");

        outputFile << valueSame32 << "\t";
        if(maxValue<valueSame32)
            maxValue = valueSame32;

        double valueSame64 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");

        outputFile << valueSame64 << "\t";
        if(maxValue<valueSame64)
            maxValue = valueSame64;
        double valueSame128 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");

        outputFile << valueSame128 << "\t";
        if(maxValue<valueSame128)
            maxValue = valueSame128;
        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */
        double valuediff16 = getMaxMinValue(resultsByClass[i].diff_class_distances_16 , "min");

        outputFile << valuediff16 << "\t";
        if(maxValue<valuediff16)
            maxValue = valuediff16;
        double valuediff32 = getMaxMinValue(resultsByClass[i].diff_class_distances_32 , "min");

        outputFile << valuediff32 << "\t";
        if(maxValue<valuediff32)
            maxValue = valuediff32;
        double valuediff64 = getMaxMinValue(resultsByClass[i].diff_class_distances_64 , "min");

        outputFile << valuediff64 << std::endl;
        if(maxValue<valuediff64)
            maxValue = valuediff64;
    }
    double error = 100, umbralOptimo;
    for(double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
        double FP=0, FN=0;
        for(int i = 0; i < resultsByClass.size(); i++) {

            /* Get the max value and min value for a vector of double */
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_16 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            /* Get the max value and min value for a vector of double */
            valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_16 , "min");
            if(valueSame32<umbral){
                FN++;
            }
        }
        if(error> ((FP+FN)/(2 * imageClassesDirectories.size()))){
            error = ((FP+FN)/(2 * imageClassesDirectories.size()));
            umbralOptimo = umbral;
        }
    }
    outputFile << "ERROR 16  = "  << "\t";

    outputFile << error << "\t";
    outputFile << "UMBRAL OPTIMO = "  << "\t";

    outputFile << umbralOptimo << "\t";

    error = 100;
    for(double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
        double FP=0, FN=0;
        for(int i = 0; i < resultsByClass.size(); i++) {

            /* Get the max value and min value for a vector of double */
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            /* Get the max value and min value for a vector of double */
            valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_32 , "min");
            if(valueSame32<umbral){
                FN++;
            }
        }
        if(error> ((FP+FN)/(2 * imageClassesDirectories.size()))){
            error = ((FP+FN)/(2 * imageClassesDirectories.size()));
            umbralOptimo = umbral;
        }
    }
    outputFile << "ERROR 32  = "  << "\t";

    outputFile << error << "\t";
    outputFile << "UMBRAL OPTIMO = "  << "\t";

    outputFile << umbralOptimo << "\t";

    error = 100;
    for(double umbral = 0; umbral < maxValue; umbral = umbral + (maxValue/10000)) {
        double FP=0, FN=0;
        for(int i = 0; i < resultsByClass.size(); i++) {

            /* Get the max value and min value for a vector of double */
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            /* Get the max value and min value for a vector of double */
            valueSame32 = getMaxMinValue(resultsByClass[i].diff_class_distances_64 , "min");
            if(valueSame32<umbral){
                FN++;
            }
        }
        if(error> ((FP+FN)/(2 * imageClassesDirectories.size()))){
            error = ((FP+FN)/(2 * imageClassesDirectories.size()));
            umbralOptimo = umbral;
        }
    }
    outputFile << "ERROR 64  = "  << "\t";

    outputFile << error << "\t";
    outputFile << "UMBRAL OPTIMO = "  << "\t";

    outputFile << umbralOptimo << "\t";
    outputFile << "alfa y beta = " << "\t";
    outputFile << alfa << "-" << 1-alfa << "\t";

    outputFile.close();
}
#endif //CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
