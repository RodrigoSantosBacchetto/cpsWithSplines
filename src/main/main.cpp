
#include "main.hpp"



std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);

int main() {

    double alfa = 1;
    double beta = 1;
    int sample = 9;


    cv::Mat image= cv::imread("C:\\Users\\Santos\\Desktop\\spoon-4.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i > hir;
    cv::Mat contourOutput = image.clone();
    cv::Mat mathEdge;
    cv::threshold(contourOutput, mathEdge, 192.0, 255.0, CV_THRESH_BINARY_INV);
    cv::findContours( contourOutput, contours, hir, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    int pointsNumber = 0;

    int countContourPoints = 0;

    int points = sample;

    double X[points];
    double Y[points];

    std::vector<cv::Point> contour = extractContourPoints(contours, sample);

    MatrixXd mta = generateCpsWithSplineRefinement(points, contour, contours);


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




