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
/*void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories);*/

struct CustomLessThan
{
    bool operator()(element const &lhs, element const &rhs) const
    {
        return lhs.distance < rhs.distance;
    }
};

void largeDeformationExperimentWithSplineCps(std::vector<std::string> imageClassesDirectories, std::string imageReferenceDirectories ) {
    std::vector<classResults> resultsByClass;
    //Explore class folders

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\cps_plus_splines.csv", std::ios_base::out);
    double alfa = 0.5;
    classResults finalResult;
    std::vector<cv::Mat> referenceImages = readImagesFromDirectory(imageReferenceDirectories);

    for (int i = 0; i < imageClassesDirectories.size(); i++) {
        std::vector<cspResult> cpsResults;
        std::cout << std::endl << "Started processing folder [ " << i << " ]: " << getClassNameFromPath(imageClassesDirectories[i]);
        std::vector<cv::Mat> folderImages = readImagesFromDirectory(imageClassesDirectories[i]);
        for (int j = 0; j < folderImages.size() ; j++) {
            /*Simple image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(folderImages[j], ONLY_EXTERNAL_CONTOUR);
            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));
            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cspResult simpleImageCpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            cpsResults.push_back(simpleImageCpsMatrix);
        }
        finalResult.cspMatrix.push_back(cpsResults);
    }

    for (int r = 0; r < referenceImages.size(); r++) {
        std::vector<cv::Point> referenceFullContour = getKuimContour(referenceImages[r], ONLY_EXTERNAL_CONTOUR);
        std::cout << std::endl << "Started processing  [ " << r << " ]: ";
        /* Calculate the area for the contour in order to normalize*/
        const double referenceArea = sqrt(contourArea(referenceFullContour));
        std::vector<cv::Point> referenceSampledPoints = sampleContourPoints(referenceFullContour, 128);
        cspResult referenceCpsMatrix = generateCpsWithSplineRefinement(referenceSampledPoints, referenceArea);


        resultByClass referenceResult;
        referenceResult.name =  getClassNameFromPath(imageClassesDirectories[r]);
        // For each pruebas's folder, we need to iterate.
        for (int i = 0; i < imageClassesDirectories.size(); i++) {

            std::vector<cv::Mat> folderImages = readImagesFromDirectory(imageClassesDirectories[i]);

            for (int j = 0; j < folderImages.size() ; j++) {
                element image;
                if(i == r){
                    image.belongs = true;
                }else{
                    image.belongs = false;
                }
                image.distance = similarityMeasure(referenceCpsMatrix, finalResult.cspMatrix[i][j], alfa, 1 - alfa);
                image.folderNumber = getClassNameFromPath(imageClassesDirectories[i]);
                image.number = j;
                referenceResult.simpleImage.push_back(image);
            }
        }
        finalResult.divisions.push_back(referenceResult);
    }


    outputFile << "\tClass Folder Name\tResult" << std::endl;
    for (int r = 0; r < finalResult.divisions.size(); r++) {
        std::vector<element> tempElementResult = finalResult.divisions[r].simpleImage;
        for (int i = 0; i < tempElementResult.size(); i++) {
            std::sort(tempElementResult.begin(), tempElementResult.end(), CustomLessThan());
        }
        if(r==1) {
            for (int i = 0; i < tempElementResult.size(); i++) {
                std::cout  << "element : " << tempElementResult[i].distance << std::endl;
            }

        }

        for (int i = 0; i < 25; i++) {
            if(tempElementResult[i].belongs) {
                finalResult.divisions[r].hits++;
            }
        }
        outputFile << finalResult.divisions[r].name << "\t";
        outputFile << finalResult.divisions[r].hits/25 << "\t";
        for (int i = 0; i < 25; i++) {
            if(!tempElementResult[i].belongs) {
                outputFile << tempElementResult[i].folderNumber << " - " << tempElementResult[i].number << "\t";
            }
        }
        outputFile << std::endl;

    }
    outputFile.close();
}


/*void largeDeformationExperimentWithOriginalCps(std::vector<std::string> imageClassesDirectories) {
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
            *//*If j=0 calc the first image cps data*//*
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                *//* Calculate the area for the contour in order to normalize*//*
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

            *//*Similar image*//*
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j+1], ONLY_EXTERNAL_CONTOUR);

            *//* Calculate the area for the contour in order to normalize*//*
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

            *//*different image*//*
            std::vector<cv::Point> fullContourDifferent = getKuimContour(currentResult.images[j+6], ONLY_EXTERNAL_CONTOUR);

            *//* Calculate the area for the contour in order to normalize*//*
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

        *//* Get the max value and min value for a vector of double *//*
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

        *//* Get the max value and min value for a vector of double *//*
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

            *//* Get the max value and min value for a vector of double *//*
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_16 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            *//* Get the max value and min value for a vector of double *//*
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

            *//* Get the max value and min value for a vector of double *//*
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            *//* Get the max value and min value for a vector of double *//*
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

            *//* Get the max value and min value for a vector of double *//*
            double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");
            if(valueSame32>umbral){
                FP++;
            }

            *//* Get the max value and min value for a vector of double *//*
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
}*/
#endif //CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
