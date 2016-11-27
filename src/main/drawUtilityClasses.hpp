//
// Created by Santos on 11/27/2016.
//


#ifndef CPSWITHSPLINES_DRAWUTILITYCLASSES_H
#define CPSWITHSPLINES_DRAWUTILITYCLASSES_H

#include "main.hpp"

void drawNow(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, std::vector<std::vector<cv::Point>> vector);
cv::Mat generateSplineBasedFigure(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, int , int );
void printNewSample (std::vector<cv::Point> pointsToDraw);


/**
 * This method use the results matrix for X and Y to draw all the figure
 */
void drawNow(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, std::vector<std::vector<cv::Point>> contours) {
    // Create black empty images
    IplImage* img = cvCreateImage( cvSize( 640, 480 ), 8, 1 );

    int uVariation = 0;

    CvPoint cvPoints[resultsMatrixX.rows()*(int)(1/0.001)];
    /*Calculate all the points in the contour*/
    for(int row=0;row<=resultsMatrixX.rows()-1;row++){
        for(double u=0;u<=1;u+=0.001){
            cvPoints[uVariation] = cvPoint((int)(round((resultsMatrixX(row,0)*pow(u,3)) + (resultsMatrixX(row,1)*pow(u,2)) + (resultsMatrixX(row,2)*u) + resultsMatrixX(row,3))),(int)(round((resultsMatrixY(row,0)*pow(u,3)) + (resultsMatrixY(row,1)*pow(u,2)) + (resultsMatrixY(row,2)*u) + resultsMatrixY(row,3))));
            uVariation++;
        }
    }
    /*Draw all the points in the image*/
    for(int j=0;j<=resultsMatrixX.rows()*(1/0.001);j++){
        cvCircle(img, cvPoints[j],1,CV_RGB(255,0,0),3,1,1);
    }

    /* Draw all the points in the image*/
    int countContourPoints = 0;
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            if(contours[i][j].x > 10000 || contours[i][j].x < -1)
                break;
            countContourPoints++;
        }
    }
    int dist = 1;
    int flag = 1;
    for(int i = 0; i < contours.size(); i++){
        for(int j = 0; j < contours[i].size(); j++) {
            if(contours[i][j].x > 2000 || contours[i][j].x < -1 || contours[i][j].y > 2000 || contours[i][j].y < -1)
                break;
            if(dist == flag) {
                cvCircle(img, cvPoint(contours[i][j].x,contours[i][j].y),1,CV_RGB(250,244,227),3,1,1);
                flag = 0;
            }
            flag++;
        }
    }



    cvShowImage("Image",img);
    cvWaitKey( 0 );
}


cv::Mat generateSplineBasedFigure(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, int originalImgRows, int originalImgCols) {
    IplImage* img = cvCreateImage( cvSize( originalImgRows, originalImgCols ), 8, 1 );

    std::vector<CvPoint> cvPoints;
    /*Calculate all the points in the contour*/
    int lastX = -1, lastY = -1, uIndex = 0;
    int eqCount = resultsMatrixX.rows();
    for(int row = 0; row < eqCount; row++){
        for(double u=0; u <= 1; u+=0.00001){
            int XCoordinate = (int)(round((resultsMatrixX(row,0)*pow(u,3)) + (resultsMatrixX(row,1)*pow(u,2)) + (resultsMatrixX(row,2)*u) + resultsMatrixX(row,3)));
            int YCoordinate = (int)(round((resultsMatrixY(row,0)*pow(u,3)) + (resultsMatrixY(row,1)*pow(u,2)) + (resultsMatrixY(row,2)*u) + resultsMatrixY(row,3)));


            cvPoints.push_back(cvPoint(XCoordinate, YCoordinate));
            uIndex++;
            lastX = XCoordinate;
            lastY = YCoordinate;

        }
    }

    /*Draw all the points that were inserted*/
    for(int j = 0; j <= uIndex; j++) {
        cvCircle(img, cvPoints[j], 1, CV_RGB(255, 255, 255), 1, 1, 1);
    }

    return cv::cvarrToMat(img);
}

void printNewSample (std::vector<cv::Point> pointsToDraw) {
    IplImage* imageToPrint = cvCreateImage(cvSize(500,500), 8, 3);

    for(int i = 0; i < pointsToDraw.size(); i++) {
        cvCircle(imageToPrint, pointsToDraw[i],1, CV_RGB(255, 255, 255), 1, 1, 1);
    }

    std::stringstream s;
    s << "sample_" << std::rand();
    cv::imshow(s.str(),cv::cvarrToMat(imageToPrint));
    cvWaitKey();
}

#endif //CPSWITHSPLINES_DRAWUTILITYCLASSES_H
