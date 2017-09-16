//
// Created by Santos on 11/27/2016.
//

#ifndef CPSWITHSPLINES_CONTOURUTILITIES_H
#define CPSWITHSPLINES_CONTOURUTILITIES_H

#include "main.hpp"
#include "generalFunctions.hpp"

std::vector<cv::Point> extractContourPoints(std::vector<std::vector<cv::Point>> vector, int sample);
std::vector<cv::Point> getKuimContour (cv::Mat, int);
std::vector<cv::Point> sampleContourPoints(std::vector<cv::Point>, int);
std::vector<cv::Point> samplePointsFromSpline(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, int sampleSize);

/**
 * This method convert a contour of points into a vector of points.
 */
std::vector<cv::Point> extractContourPoints(std::vector<std::vector<cv::Point>> contours, int sample) {
    std::vector<cv::Point> newSample;
    int pointsNumber = 0;
    int countContourPoints = 0;
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            if(contours[i][j].x > 10000 || contours[i][j].x < -1)
                break;
            countContourPoints++;
        }
    }
    int dist = countContourPoints/sample;
    int flag = countContourPoints/sample;
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            if(contours[i][j].x > 2000 || contours[i][j].x < -1 || contours[i][j].y > 2000 || contours[i][j].y < -1)
                break;
            if(dist == flag) {
                cv::Point_<double> tempPoint = cvPoint2D32f(contours[i][j].x, contours[i][j].y);
                newSample.push_back(tempPoint);
                pointsNumber++;
                flag = 0;
            }
            flag++;
        }
    }
    return newSample;
}

std::vector<cv::Point> getKuimContour(cv::Mat originalImage, int numberOfContours) {

    cv::Mat data1;
    cv::threshold(originalImage, data1, 192.0, 255.0,CV_THRESH_BINARY);

    std::vector<cv::Point> contour;
    int i = 0, j = 0, k = numberOfContours - 1;

    // add black borders to our image
    copyMakeBorder(data1, data1, 2, 2, 2, 2, IPL_BORDER_CONSTANT, CV_RGB(0,0,0) );

    int totalRows = data1.rows;
    int totalCols = data1.cols;

    IplImage* allBlack = cvCreateImage(cvSize(totalCols, totalRows), 8, 1);
    cvSetZero(allBlack);
    cv::Mat data2 = cv::cvarrToMat(allBlack);

    // First, we search for a starting position
    for(int sy = 0; sy < totalRows; sy++){
        for(int sx = 0; sx < totalCols; sx++){
            if (((int)(data1.at<uchar>(sy,sx)) == 255)
                && ((sx == 1) || ((int)(data1.at<uchar>(sy,sx - 1)) == 0))){

                //When a valid starting point is reached, prepare to track contour
                int x = sx;
                int y = sy;
                data1.at<uchar>(y,x) = (uchar)(MINUS_ONE);
                data2.at<uchar>(y,x) = 255;
                int last = 0;
                int next = getNext(x, y, last, data1, totalRows, totalCols);

                k++;

                // Track contour counter clockwise
                while (( (int)(data1.at<uchar>(y + dy[next], x + dx[next])) > 100 ) && (k==1)){

                    y = y + dy[next];
                    x = x + dx[next];
                    data1.at<uchar>(y,x) = (uchar)(DOS);
                    data2.at<uchar>(y,x) = 255;

                    last = (next + 4) % 8;
                    next = getNext(x, y, last, data1, totalRows, totalCols);

                    cv::Point contourPoint = cvPoint(x,y);
                    contour.push_back(contourPoint);

                }
            }
        }
    }

    cvReleaseImage(&allBlack);

    return contour;


}


std::vector<cv::Point> sampleContourPoints(std::vector<cv::Point> fullContour, int sampleSize) {
    std::vector<cv::Point> sampledPoints;

    double delta = (double)fullContour.size() / (double)sampleSize;
    for( double i = 0; i < fullContour.size(); i += delta)
        if(sampledPoints.size()<sampleSize) {
            sampledPoints.push_back(fullContour[round(i)]);
        }

    return sampledPoints;
}

std::vector<cv::Point> samplePointsFromSpline(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, int sampleSize) {

    double perimeter = 0;
    for (int f = 0; f < resultsMatrixX.rows(); f++) {
        //eq coefficients
        double ax = resultsMatrixX(f,0);
        double bx = resultsMatrixX(f,1);
        double cx = resultsMatrixX(f,2);
        double dx = resultsMatrixX(f,3);

        double ay = resultsMatrixY(f,0);
        double by = resultsMatrixY(f,1);
        double cy = resultsMatrixY(f,2);
        double dy = resultsMatrixY(f,3);

        double lastX = dx;
        double lastY = dy;

        for(double t = 0.0001; t <= 1; t += 0.0001){
            double thisX = (ax * pow(t,3)) + (bx * pow(t,2)) + (cx * t) + dx;
            double thisY = (ay * pow(t,3)) + (by * pow(t,2)) + (cy * t) + dy;

            perimeter += sqrt( pow((thisX - lastX),2) + pow((thisY - lastY),2));

            lastX = thisX;
            lastY = thisY;
        }
    }

    double spacingNeeded = perimeter / (double)sampleSize;
    std::vector<cv::Point> sampledPoints;

    double currentSpacing = 0;
    for (int f = 0; f < resultsMatrixX.rows(); f++) {
        //eq coefficients
        double ax = resultsMatrixX(f, 0);
        double bx = resultsMatrixX(f, 1);
        double cx = resultsMatrixX(f, 2);
        double dx = resultsMatrixX(f, 3);

        double ay = resultsMatrixY(f, 0);
        double by = resultsMatrixY(f, 1);
        double cy = resultsMatrixY(f, 2);
        double dy = resultsMatrixY(f, 3);

        double lastX = dx;
        double lastY = dy;

        if (sampledPoints.size() == 0) {
            sampledPoints.push_back(cvPoint((int) round(dx), (int) round(dy)));
        }

        for (double t = 0.0001; t <= 1; t += 0.0001) {
            double thisX = (ax * pow(t, 3)) + (bx * pow(t, 2)) + (cx * t) + dx;
            double thisY = (ay * pow(t, 3)) + (by * pow(t, 2)) + (cy * t) + dy;

            currentSpacing += sqrt(pow((thisX - lastX), 2) + pow((thisY - lastY), 2));

            if (currentSpacing >= spacingNeeded) {
                currentSpacing = 0;
                sampledPoints.push_back(cvPoint((int) round(thisX), (int) round(thisY)));

                if (sampledPoints.size() == sampleSize) {
                    //when the sample has the desired size, break the loops
                    t = 1.1;
                    f = resultsMatrixX.rows();
                }
            }
            lastX = thisX;
            lastY = thisY;
        }
    }

    return sampledPoints;
}


#endif //CPSWITHSPLINES_CONTOURUTILITIES_H
