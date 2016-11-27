
#include "main.hpp"

typedef struct classResults {
    std::string class_name;
    std::vector<cv::Mat> images;

    std::vector<MatrixXd> cp_signatures_32; // n
    std::vector<double> same_class_distances_32; // (n-1) / 2
    std::vector<double> diff_class_distances_32; // (n-1) / 2

    std::vector<MatrixXd> cp_signatures_64;
    std::vector<double> same_class_distances_64;
    std::vector<double> diff_class_distances_64;

    std::vector<MatrixXd> cp_signatures_128;
    std::vector<double> same_class_distances_128;
    std::vector<double> diff_class_distances_128;

} classResults;

void experiment1_originalCps(std::vector<std::string> imageClassesDirectories);
void experiment1_splineCps(std::vector<std::string> imageClassesDirectories);

int main() {

    std::string parentDirectory = "C:\\Users\\Santos\\Desktop\\pruebaR\\";
    std::vector<std::string> imageClassesDirectories = getClassDirectories(parentDirectory);
/*

    experiment1_originalCps(imageClassesDirectories);
*/
    experiment1_splineCps(imageClassesDirectories);

    cvWaitKey( 0 );
//    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
//    std::vector<cv::Point> fullContour = getKuimContour(allImages[0], ONLY_EXTERNAL_CONTOUR);
//    std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, sample);
//
//    MatrixXd mta = generateCpsWithSplineRefinement(sampledPoints);
//    MatrixXd mtb = generateCpsWithSplineRefinement(sampledPoints);
//
//    /* the first value is the column index with the minimum sum*/
//    /* the second value is the max distance between two matrix*/
//    /* the third value is the promedian distance between two matrix*/
//    std::vector<double> dist = smCpsRm( mta, mtb);
    cvWaitKey();
}


void experiment1_splineCps(std::vector<std::string> imageClassesDirectories) {
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

                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                /* Calculate the area for the contour in order to normalize*/
                std::vector<cv::Point> sampledPoints2 = sampleContourPoints(fullContour, 256);
                printNewSample(sampledPoints2);
            /* Calculate the area for the contour in order to normalize*/
                const double area = sqrt(contourArea(fullContour));
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 256);
                MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
/*
                sampledPoints = sampleContourPoints(fullContour, 32);

                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                currentResult.cp_signatures_64.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 64);
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, area);
                currentResult.cp_signatures_128.push_back(cpsMatrix);
                std::cout << std::endl << j;
*/

 /*           *//*Similar image*//*
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j+1], ONLY_EXTERNAL_CONTOUR);

            *//* Calculate the area for the contour in order to normalize*//*
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 16);
            MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 32);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 64);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints, areaSimilar);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrix)[1]);
 */       }
        cvWaitKey( 0 );
        resultsByClass.push_back(currentResult);

        std::cout << std::endl << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\output_ours.csv", std::ios_base::out);

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


void experiment1_originalCps(std::vector<std::string> imageClassesDirectories) {
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
                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 16);
                MatrixXd cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_32.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 32);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_64.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 64);
                cpsMatrix = computeCps(sampledPoints, area);
                currentResult.cp_signatures_128.push_back(cpsMatrix);
            }
            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j + 1], ONLY_EXTERNAL_CONTOUR);

            /* Calculate the area for the contour in order to normalize*/
            const double areaSimilar = sqrt(contourArea(fullContourSimilar));

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 16);
            MatrixXd cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 32);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 64);
            cpsMatrix = computeCps(sampledPoints, areaSimilar);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrix)[1]);

        }

        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("C:\\Users\\Santos\\Desktop\\pruebaR\\output_original.csv", std::ios_base::out);

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











