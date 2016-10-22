
#include "main.hpp"



std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);

double getMaxMinValue(std::vector<double> vector, const char string[4]);

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

    std::string parentDirectory = "C:\\Users\\Santos\\Desktop\\pruebaR\\";
    std::vector<std::string> imageClassesDirectories = getClassDirectories(parentDirectory);

    std::vector<classResults> resultsByClass;
    //Explore class folders
    for(int i = 0; i < imageClassesDirectories.size(); i++){
        classResults currentResult;

        //Store class name
        currentResult.class_name = imageClassesDirectories[i];

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
            currentResult.same_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            sampledPoints = sampleContourPoints(fullContourSimilar, 128);
            cpsMatrix = generateCpsWithSplineRefinement(sampledPoints);
            currentResult.same_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrix)[1]);

            /*Different image*/
            std::vector<cv::Point> fullContourDifferent = getKuimContour(currentResult.images[j+1+(currentResult.images.size()-1)/2], ONLY_EXTERNAL_CONTOUR);

            std::vector<cv::Point> sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 32);
            MatrixXd cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_32.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrixDifferent)[1]);

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 64);
            cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_64.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrixDifferent)[1]);

            sampledPointsDifferent = sampleContourPoints(fullContourDifferent, 128);
            cpsMatrixDifferent = generateCpsWithSplineRefinement(sampledPointsDifferent);
            currentResult.diff_class_distances_128.push_back(smCpsRm(currentResult.cp_signatures_32[0],cpsMatrixDifferent)[1]);
        }

        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
    }

    for(int i = 0; i < imageClassesDirectories.size(); i++) {
        /* Get the max value and min value for a vector of double */
        double valueDiff = getMaxMinValue(resultsByClass[i].diff_class_distances_32 , "min");
        double valueSame = getMaxMinValue(resultsByClass[i].same_class_distances_32 , "max");
    }
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

/**
 * This method get the max or min value on a vector of doubles.
 */
double getMaxMinValue(std::vector<double> vector, std::string valueType) {
    double tempVal = vector[0];
    for(int i = 0; i < vector.size(); i++){
        if("min"==valueType && tempVal>vector[i]){
            tempVal = vector[i];
        } else if("max"==valueType && tempVal<vector[i]){
            tempVal = vector[i];
        }
    }
    return tempVal;
}


/**
 * This method get the distance between two cps matrix.
 */
std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb) {
    std::vector<double> result;
    /* Number of point samples*/
    double n = mta.rows();
    MatrixXd matrix((int)n,(int)n);

     /*Each value of k represent a different rotation*/
    for(int k = 0; k <  n; k++) {
        std::vector<int> vector;
        for (double u = k - 1; u < n-1 + k; u++) {
            if(u >= n-1)
                vector.push_back((int)fmod( u , n-1));
            else
                vector.push_back((int)fmod( u , n-1)+1);

        }
         /*Calculate the euclidian distance*/
        for(int i = 0; i <  n; i++) {
            double sumDist = 0;
            for(int j = 0; j < n; j++) {
                sumDist += (mta(i,j)-mtb(vector[i],j)) * (mta(i,j)-mtb(vector[i],j));
            }
            matrix(i,k) = sqrt(sumDist);
        }
    }
     /*the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = minSum(matrix);
    result.push_back(matchingData.y);
    result.push_back(matchingData.x/mta.rows());
    double maxValue = matrix(0,matchingData.y);
    for(int i = 0; i <  n; i++) {
        if(matrix(i,matchingData.y)>maxValue){
            maxValue = matrix(i,matchingData.y);
        }
    }
    result.push_back(maxValue);

    return result;
}




