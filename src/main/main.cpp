
#include "main.hpp"



std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);

int main() {

    double alfa = 1;
    double beta = 1;
    int sample = 20;


    cv::Mat image= cv::imread("D:\\FP-UNA\\Image Databases\\spoon-1.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<cv::Point> fullContour = getKuimContour(image, ONLY_EXTERNAL_CONTOUR);
    std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, sample);

    MatrixXd mta = generateCpsWithSplineRefinement(sampledPoints);

    cvWaitKey();
  /*  std::vector<double> dist = smCpsRm( mta, mtb);*/
}



/**
 * This method get the distance between two cps matrix.
 *//*
std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb) {
    std::vector<double> dist;
    int f = mta.rows();
    for(int i = 0; i < f; i++){

        for(int k = 0; k < f; k++){

        }

    }
    return dist;
}*/




