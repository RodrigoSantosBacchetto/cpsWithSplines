
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

int main() {

    double alfa = 1;
    double beta = 1;
    int sample = 20;

    std::string parentDirectory = "D:\\FP-UNA\\Image Databases\\ForExperiments\\rotatedAndScaledClasses\\";
    std::vector<std::string> imageClassesDirectories = getClassDirectories(parentDirectory);

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

        for(int j = 0; j < (currentResult.images.size()-1)/2; j++){
            /*If j=0 calc the first image cps data*/
            if(j==0) {
                std::vector<cv::Point> fullContour = getKuimContour(currentResult.images[j], ONLY_EXTERNAL_CONTOUR);

                std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, 32);
                MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
                currentResult.cp_signatures_32.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 64);
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
                currentResult.cp_signatures_64.push_back(cpsMatrix);

                sampledPoints = sampleContourPoints(fullContour, 128);
                cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
                currentResult.cp_signatures_128.push_back(cpsMatrix);
            }
            /*Similar image*/
            std::vector<cv::Point> fullContourSimilar = getKuimContour(currentResult.images[j+1], ONLY_EXTERNAL_CONTOUR);

            std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContourSimilar, 32);
            MatrixXd cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
            currentResult.same_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 64);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrix)[1]);

            /*Different image*/
            std::vector<cv::Point> fullContourDifferent = getKuimContour(currentResult.images[j+1+(currentResult.images.size()-1)/2], ONLY_EXTERNAL_CONTOUR);

            std::vector<cv::Point> sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 32);
            MatrixXd cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrixDifferent)[1]);

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 64);
            cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_64[0],cpsMatrixDifferent)[1]);

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 128);
            cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_128[0],cpsMatrixDifferent)[1]);
        }

        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    std::fstream outputFile;
    outputFile.open("D:\\FP-UNA\\Image Databases\\ForExperiments\\rotatedAndScaledClasses\\output.csv", std::ios_base::out);

    outputFile << "CLASS\t32_HIT_MAX_DISTANCE\t32_MISS_MIN_DISTANCE\t64_HIT_MAX_DISTANCE\t64_MISS_MIN_DISTANCE\t128_HIT_MAX_DISTANCE\t128_MISS_MIN_DISTANCE" << std::endl;

    for(int i = 0; i < resultsByClass.size(); i++) {

        outputFile << resultsByClass[i].class_name << "\t";

        /* Get the max value and min value for a vector of double */
        double valueDiff32 = getMaxMinValue(resultsByClass[i].diff_class_distances_32 , "min");
        double valueSame32 = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");

        outputFile << valueSame32 << "\t" << valueDiff32 << "\t";

        double valueDiff64 = getMaxMinValue(resultsByClass[i].diff_class_distances_64 , "min");
        double valueSame64 = getMaxMinValue(resultsByClass[i].same_class_distances_64 , "max");

        outputFile << valueSame64 << "\t" << valueDiff64 << "\t";

        double valueDiff128 = getMaxMinValue(resultsByClass[i].diff_class_distances_128 , "min");
        double valueSame128 = getMaxMinValue(resultsByClass[i].same_class_distances_128 , "max");

        outputFile << valueSame128 << "\t" << valueDiff128 << std::endl;

    }

    outputFile.close();
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






