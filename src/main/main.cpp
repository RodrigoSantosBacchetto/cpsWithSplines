
#include "main.hpp"



std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);

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

    std::string parentDirectory = "D:\\FP-UNA\\Image Databases\\ForExperiments\\EXP01_splitByClasses\\";
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
        int asd = currentResult.images.size();
        for(int j = 0; j < asd; j++){
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

        resultsByClass.push_back(currentResult);

        std::cout << "Finished processing class [ " << i << " ]: " << currentResult.class_name << std::endl;
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
    double minValue = matrix(0,matchingData.y);
    for(int i = 0; i <  n; i++) {
        if(matrix(i,matchingData.y)<minValue){
            minValue = matrix(i,matchingData.y);
        }
    }
    result.push_back(minValue);

    return result;
}




