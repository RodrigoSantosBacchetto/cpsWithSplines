
#include "main.hpp"

int main() {

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

    int pointsNumber = 0;

/*    std::cout << "x1"<< std::endl << contours[0][0] << std::endl;
    std::cout << "x2"<< std::endl << contours[1][0] << std::endl;
    std::cout << "x3"<< std::endl << contours[2][0] << std::endl;*/
    int asdaps = sample;
    for(int i = 0; i <  contours.size(); i= i+ (int)(contours.size()/sample)){
        std::cout << "X" + i<< std::endl << contours[i][0].x << std::endl;
        X[pointsNumber] = contours[i][0].x;
        Y[pointsNumber] = contours[i][0].y;
        std::cout << "Y" + i<< std::endl << contours[i][0].y << std::endl;
        pointsNumber++;
    }

    cvx::CpsMatrix mta = generateCpsWithSplineRefinement(points, X, Y, contours);

}