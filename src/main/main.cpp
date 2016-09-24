
#include "main.hpp"

int main() {

    double alfa = 1;
    double beta = 1;
    int sample = 20;


    cv::Mat image= cv::imread("C:\\Users\\Santos\\Desktop\\spoon-4.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = image.clone();
    cv::Mat mathEdge;
    cv::threshold(contourOutput, mathEdge, 192.0, 255.0, CV_THRESH_BINARY_INV);
    cv::findContours( contourOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    int points = sample;

    double X[points];
    double Y[points];

    /*Extract points into arrays*/
    convertPoints(X, Y, contours, sample);

    cvx::CpsMatrix mta = generateCpsWithSplineRefinement(points, X, Y, contours);
    cvx::CpsMatrix mtb = generateCpsWithSplineRefinement(points, X, Y, contours);

    /* TODO: struct of this.
     * the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = matchingCps( mta, mtb);
    /* Result distance of point matching*/
    double dist1 = matchingData.x;

    /*Metric 2 implementation in progress*/

    /* Result distance of afin transformation*/
    double dist2 = 1;

    double finalDisimilarityDistance = dist1*alfa + beta*dist2;
}




