//
// Created by Santos on 9/19/2016.
//

#ifndef CPSWITHSPLINES_MAIN_H
#define CPSWITHSPLINES_MAIN_H

/*CPS libraries*/
#include <CpSignature.hpp>
#include <CpsMatrix.hpp>
/*common files*/
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define MINUS_ONE 99
#define DOS 150

using namespace Eigen;

typedef Eigen::SparseMatrix<double> sMatrix;

/*Functions prototype declaration*/
void drawNow(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, std::vector<std::vector<cv::Point>> vector);
MatrixXd generateCpsWithSplineRefinement(int points, std::vector<cv::Point> X,
                                               std::vector<std::vector<cv::Point>> Y);
cv::Point2d matchingCps(cvx::CpsMatrix cpsA, cvx::CpsMatrix cpsB);
cv::Point2d minSum(cv::Mat mat);
void convertPoints(double X[], double Y[], std::vector<std::vector<cv::Point>> contours, int sample);
std::vector<cv::Point> extractContourPoints(std::vector<std::vector<cv::Point>> vector, int sample);

void getSortedContour(cv::Mat);
std::vector<cv::Point> getKuimContour (cv::Mat, int);
int getNext(int x, int y, int last, cv::Mat data, int totalRows, int totalCols);

int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

/*Functions implementation*/
MatrixXd computeCps(std::vector<cv::Point> contourPoints, const double area);

/**
 * This method is used to generate the cps matrix, using the cubic spline function constructed with the countour points.
 * TODO: Implement a class with this functions and attributes.
 */
MatrixXd generateCpsWithSplineRefinement(int points, std::vector<cv::Point> vector, std::vector<std::vector<cv::Point>> contours) {

    // Define aux arraysfac

    sMatrix Coefficients(points,points);

    VectorXd XRightHand(points);
    VectorXd YRightHand(points);

    VectorXd XResults(points);
    VectorXd YResults(points);

    Coefficients.reserve(VectorXd::Constant(points,3));

    int iPlusOne, iMinusOne;

    for(int i = 0; i <  points; i++){

        iPlusOne = (i+1)%points;
        iMinusOne = (i == 0) ? (points - 1) : (i - 1);

        // -----------------------------------------------------------------------------------
        // Build the matrix with first derivatives as unknown values
        // -----------------------------------------------------------------------------------

        Coefficients.insert( i , iMinusOne ) = 1;
        Coefficients.insert( i , i ) = 4;
        Coefficients.insert( i , iPlusOne ) = 1;

        // -----------------------------------------------------------------------------------
        // Build vectors with right-hand values for X and Y
        // -----------------------------------------------------------------------------------

        XRightHand( i ) = 3 * (vector[ iPlusOne ].x - vector[ iMinusOne ].x);
        YRightHand( i ) = 3 * (vector[ iPlusOne ].y - vector[ iMinusOne ].y);
    }

    // Compress Sparse matrix -- This is required by the equation solver
    Coefficients.makeCompressed();

    // Solve equation systems
    SparseLU<sMatrix> eqSolver;

    eqSolver.compute(Coefficients);
    if(eqSolver.info() != Success){
        // Matrix decomposition failed!
        std::cout << "There was an error de-composing the matrix" << std::endl << eqSolver.info();
        //XResults << -9999;
    }

    XResults = eqSolver.solve(XRightHand);
   if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for X" << std::endl << eqSolver.info();
        //XResults << -9999;
    }

    YResults = eqSolver.solve(YRightHand);
    if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for Y" << std::endl << eqSolver.info();
        //XResults << -9999;
    }


    // Display data and results
    std::cout << "Coefficients matrix:" << std::endl << Coefficients << std::endl;

    std::cout << "X - Right Hand:" << std::endl << XRightHand << std::endl;
    std::cout << "X - Results:" << std::endl << XResults << std::endl;

    std::cout << "Y - Right Hand:" << std::endl << YRightHand << std::endl;
    std::cout << "Y - Results:" << std::endl << YResults << std::endl;

    // Build return matrix
    //      --> Col 0: cubic coefficient
    //      --> Col 1: quadratic coefficient
    //      --> Col 2: linear coefficient
    //      --> Col 3: independent term && starting X/Y point
    //      --> Col 4: ending X/Y point
    MatrixXd resultsMatrixX(points,5), resultsMatrixY(points,5);
    int jPlusOne;

    for (int j = 0; j < points; j++) {

        jPlusOne = (j+1)%points;

        resultsMatrixX(j,0) = 2 * (vector[ j ].x - vector[ jPlusOne ].x) + XResults(j) + XResults(jPlusOne);
        resultsMatrixX(j,1) = 3 * (vector[ jPlusOne ].x - vector[ j ].x) - 2 * XResults(j) - XResults(jPlusOne);
        resultsMatrixX(j,2) = XResults(j);
        resultsMatrixX(j,3) = vector[j].x;
        resultsMatrixX(j,4) = vector[ jPlusOne ].x;

        resultsMatrixY(j,0) = 2 * (vector[ j ].y - vector[ jPlusOne ].y) + YResults(j) + YResults(jPlusOne);
        resultsMatrixY(j,1) = 3 * (vector[ jPlusOne ].y - vector[ j ].y) - 2 * YResults(j) - YResults(jPlusOne);
        resultsMatrixY(j,2) = YResults(j);
        resultsMatrixY(j,3) = vector[j].y;
        resultsMatrixY(j,4) = vector[ jPlusOne ].y;
    }


/*    *//* Debug function, used to draw the cubic spline using the splines functions*//*
    drawNow(resultsMatrixX,
            resultsMatrixY,
            contours);*/

    std::cout << "X - Final Matrix:" << std::endl << resultsMatrixX << std::endl;
    std::cout << "Y - Final Matrix:" << std::endl << resultsMatrixY << std::endl;

    std::cout << std::endl << resultsMatrixX(0,0) + resultsMatrixX(0,1) + resultsMatrixX(0,2) + resultsMatrixX(0,3);

    /* Calculate the area for the contour in order to normalize*/
    const double area = sqrt(contourArea(vector));
    return computeCps(vector, area);
}

/**
 * Create the cps signature for a specific contour.
 */
MatrixXd computeCps(std::vector<cv::Point> contourPoints, const double area) {
    MatrixXd cps(contourPoints.size(),contourPoints.size());
    MatrixXd aux(contourPoints.size(),contourPoints.size());
    /*"initialize with "0"*/
    for(int i=0 ; i <= contourPoints.size() ; i++ ){
        for(int j=0 ; j <= contourPoints.size() ; j++ ){
            aux(i,j) = 0;
        }
    }
    /* Calculate the distance between points */
    for(int i=0 ; i <= contourPoints.size() ; i++ ){
        for(int j=0 ; j <= contourPoints.size() ; j++ ){
            if(i<j){
                aux(i,j) = sqrt((contourPoints[i].x - contourPoints[j].x)^2 + (contourPoints[i].y - contourPoints[j].y)^2);
                if(aux(i,j)==0){
                    aux(i,j) = 1;
                }
            } else if(i>j){
                aux(i,j) = aux(j,i);
            }
        }
    }
    /* we need to rotate the rows i-1 places to the left  and we normalize the values*/
    for(int i=0 ; i <= contourPoints.size() ; i++ ){
        for(int j=0 ; j <= contourPoints.size() ; j++ ){
            if(j+1 > contourPoints.size()){
                cps(i,j) = 0;
            }else{
                cps(i,j) = aux(i,j+1)/sqrt(area);
            }
        }
    }

    return cps;

}

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

/**
 *
 * This method is used to get the column that has the minum sum value(saved in X) and the index of that column(saved on Y).
 * X is the result value of the metric dist_1.
 */
cv::Point2d minSum(std::vector<std::vector<double> > mat) {
    /*We get the sum of the values for each column*/
    std::vector<double> vector;
    for(double j = 0; j <  mat[0].size(); j++) {
        double sum = 0 ;
        for (double i = 0 ; i < mat.size() ; i++) {
            sum += mat[i][j];
        }
        vector[j] = sum;
    }
    /*We get the minum value in the vector and the index of that value which represents the column of the matrix with the minum value*/
    double min = vector[0];
    int index = 0;
    for(int j = 0; j <  vector.size(); j++) {
        if(vector[j]<min){
            min = vector[j];
            index = j;
        }
    }
    cv::Point2d result;
    result.x = min;
    result.y = index;
    return result;
}

/**
* This method is going to make the matching step, using the euclidian distance.(is possible to use the r_measure distance measure).
*/
cv::Point2d matchingCps(cvx::CpsMatrix cpsA, cvx::CpsMatrix cpsB){
    /* Number of point samples*/
    double n = cpsA.data().rows;
    std::vector<std::vector<double> > matrix;


    /* Each value of k represent a different rotation*/
    int vectorIndex = 0;
    std::vector<double> vector;
    for(int k = 0; k <  n; k++) {
        for (double u = k - 1; u < n + k - 2; u++) {
            vector[vectorIndex] = fmod( u , n)+1;
            vectorIndex++;
        }
        /* Calculate the euclidian distance*/
        for(int i = 0; i <  n; i++) {
            double sumDist = 0;
            for(int j = 0; j <  n; j++) {
                sumDist += pow(cpsA.data()[i,j]-cpsB.data()[vector[i],j],2);
            }
            matrix[i][k] = sqrt(sumDist);
        }
        vectorIndex = 0;
    }
    /* the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = minSum(matrix);
    return matchingData;
}

/**
 * Extract contour points in order to get a better structure to calculate the cubic spline.
 */
void convertPoints(double X[], double Y[], std::vector<std::vector<cv::Point>> contours, int sample) {
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
                X[pointsNumber] = contours[i][j].x;
                Y[pointsNumber] = contours[i][j].y;
                pointsNumber++;
                flag = 0;
            }
            flag++;
        }
    }
}

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
                std::cout << "X - Right Hand:" << std::endl << contours[i][j].x << std::endl;
                std::cout << "X - Results:" << std::endl << contours[i][j].y << std::endl;
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
    imshow("beforeBorder",data1);
    copyMakeBorder(data1, data1, 2, 2, 2, 2, CV_HAL_BORDER_CONSTANT, CV_RGB(0,0,0) );
    imshow("aferBorder",data1);

    int totalRows = data1.rows;
    int totalCols = data1.cols;

    IplImage* allBlack = cvCreateImage(cvSize(totalRows, totalCols), 8, 1);
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

    imshow("data2", data2);

    return contour;


}


int getNext(int x, int y, int last, cv::Mat data, int totalRows, int totalCols) {
    int next = (last + 2) % 8;

    int nx = x + dx[next];
    int ny = y + dy[next];

    while((next != last) && ((nx < 0) || (nx > totalCols) || (ny < 0) || (ny > totalRows) || ((int)(data.at<uchar>(ny,nx)) == 0))) {
        next = (next + 1) % 8;
        nx = x + dx[next];
        ny = y + dy[next];
    }

    return next;
}

#endif //CPSWITHSPLINES_MAIN_H
