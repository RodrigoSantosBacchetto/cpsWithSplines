

#include "filesManagementFunctions.hpp"
#include "../experiments/largeDeformationExperiment.hpp"

int main() {

    std::string parentDirectory = "C:\\Users\\Santos\\Desktop\\pruebaR\\";
    std::vector<std::string> imageClassesDirectories = getClassDirectories(parentDirectory);

    /*largeDeformationExperimentWithOriginalCps(imageClassesDirectories);*/
    largeDeformationExperimentWithSplineCps(imageClassesDirectories);

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
//    cvWaitKey();
}













