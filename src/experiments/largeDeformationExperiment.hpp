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


void bestImagenesForClass(std::vector<std::string> imageClassesDirectories);

void bestImagenesForClass(std::vector<std::string> imageClassesDirectories) {
    std::vector<classResults> resultsByClass;
    //Explore class folders

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\ImagenesSinDeformar\\bestImages.csv", std::ios_base::out);
    double alfa = 0.5;


    for (int i = 0; i < imageClassesDirectories.size(); i++) {
        classResults currentResult;

        //Store class name
        currentResult.class_name = getClassNameFromPath(imageClassesDirectories[i]);

        //Read all class images
        currentResult.images = readImagesFromDirectory(imageClassesDirectories[i]);

        //Compute results for each image of the class
        std::cout << std::endl << "Started processing class [ " << i << " ]: " << currentResult.class_name;

        for (int j = 0; j < (currentResult.images.size() - 1); j++) {
            /*If j=0 calc the first image cps data*/
            if (j == 0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j].image, ONLY_EXTERNAL_CONTOUR);
                std::cout << std::endl << "base image lenght [ " << fullContour.size() << " ]: " <<
                std::endl;
                /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 16);
                cspResult cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                currentResult.cp_signatures_16.push_back(cpsMatrix);
            }

            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j + 1].image,
                                                                       ONLY_EXTERNAL_CONTOUR);
            std::cout << std::endl << "similar image lenght [ " << fullContourSimilar.size() << " ]: " <<
            std::endl;
            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 16);
            cspResult cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_16.push_back(
                    similarityMeasure(currentResult.cp_signatures_16[0], cpsMatrix, alfa, 1-alfa));
            currentResult.same_class_path_16.push_back(currentResult.images[j + 1].imagePath);
        }
        cvWaitKey(0);
        resultsByClass.push_back(currentResult);

        std::cout << std::endl << "Finished processing class [ " << i << " ]: " << currentResult.class_name <<
        std::endl;
    }

    outputFile <<
    std::endl;
    std::vector<int> indexs;
    for (int i = 0; i < resultsByClass.size(); i++) {
        double minValue = 0;
        while (resultsByClass[i].same_class_distances_16.size() >= 6) {
            int index = 0;
            minValue = resultsByClass[i].same_class_distances_16[0];
            for (int j = 0; j < resultsByClass[i].same_class_distances_16.size(); j++) {
                /* Print for each image */
                if (minValue < resultsByClass[i].same_class_distances_16[j]) {
                    index = j;
                }
            }
            std::remove(resultsByClass[i].same_class_path_16[index].c_str());
            resultsByClass[i].same_class_distances_16.erase(resultsByClass[i].same_class_distances_16.begin()+index);
            resultsByClass[i].same_class_path_16.erase(resultsByClass[i].same_class_path_16.begin()+index);
        }

    }
    for (int i = 0; i < resultsByClass.size(); i++) {
        for (int j = 0; j < resultsByClass[i].same_class_distances_16.size(); j++) {
            /* Print for each image */
            outputFile << resultsByClass[i].same_class_distances_16[j] << "\t\t" << std::endl;
        }
    }

    outputFile.close();
}
#endif //CPSWITHSPLINES_LARGEDEFORMATIONEXPERIMENT_H
