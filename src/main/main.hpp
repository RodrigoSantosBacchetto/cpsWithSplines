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


using namespace Eigen;

typedef Eigen::SparseMatrix<double> sMatrix;

/*Functions prototype declaration*/
void drawNow(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, std::vector<std::vector<cv::Point>> vector);
cvx::CpsMatrix generateCpsWithSplineRefinement(int points, double X[], double Y[], std::vector<std::vector<cv::Point>> contours);


/*Functions implementation*/
/**
 * This method is used to generate the cps matrix, using the cubic spline function constructed with the countour points.
 * TODO: Implement a class with this functions and attributes.
 */
cvx::CpsMatrix generateCpsWithSplineRefinement(int points, double X[], double Y[],
                                               std::vector<std::vector<cv::Point>> contours) {

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

        XRightHand( i ) = 3 * (X[ iPlusOne ] - X[ iMinusOne ]);
        YRightHand( i ) = 3 * (Y[ iPlusOne ] - Y[ iMinusOne ]);
    }

    // Compress Sparse matrix -- This is required by the equation solver
    Coefficients.makeCompressed();

    // Solve equation systems
    SparseLU<sMatrix> eqSolver;

    eqSolver.compute(Coefficients);
/*    if(eqSolver.info() != Success){
        // Matrix decomposition failed!
        std::cout << "There was an error de-composing the matrix" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
    }*/

    XResults = eqSolver.solve(XRightHand);
/*    if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for X" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
    }*/

    YResults = eqSolver.solve(YRightHand);
/*    if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for Y" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
    }*/


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

        resultsMatrixX(j,0) = 2 * (X[ j ] - X[ jPlusOne ]) + XResults(j) + XResults(jPlusOne);
        resultsMatrixX(j,1) = 3 * (X[ jPlusOne ] - X[ j ]) - 2 * XResults(j) - XResults(jPlusOne);
        resultsMatrixX(j,2) = XResults(j);
        resultsMatrixX(j,3) = X[j];
        resultsMatrixX(j,4) = X[ jPlusOne ];

        resultsMatrixY(j,0) = 2 * (Y[ j ] - Y[ jPlusOne ]) + YResults(j) + YResults(jPlusOne);
        resultsMatrixY(j,1) = 3 * (Y[ jPlusOne ] - Y[ j ]) - 2 * YResults(j) - YResults(jPlusOne);
        resultsMatrixY(j,2) = YResults(j);
        resultsMatrixY(j,3) = Y[j];
        resultsMatrixY(j,4) = Y[ jPlusOne ];
    }


    /* Debug function, used to draw the cubic spline using the splines functions*/
    drawNow(resultsMatrixX,
            resultsMatrixY,
            contours);

    std::cout << "X - Final Matrix:" << std::endl << resultsMatrixX << std::endl;
    std::cout << "Y - Final Matrix:" << std::endl << resultsMatrixY << std::endl;

    std::cout << std::endl << resultsMatrixX(0,0) + resultsMatrixX(0,1) + resultsMatrixX(0,2) + resultsMatrixX(0,3);

    /*Convert the sample taked for splines function*/
    std::vector<cv::Point> newSample;
    int size = 20;
    for(int i = 0; i <  20; i+=1){
        cv::Point_<double> tempPoint = cvPoint2D32f(X[i], Y[i]);
        newSample.push_back(tempPoint);
    }

    /**
     * TODO: translate to english.
     */
    // Crea la función que computa el CPS.
    cvx::CpsEuclidean cps;
    // Crea dos matrices de CPS. Cada fila es el CPS de un punto sampleado.
    cvx::CpsEuclidean newCps;
    // Obtiene el CPS de M=128 puntos, con una definición de M=127 muestras
    // (se elimina la columna con ceros). La matriz resultante es de 128x127
    // elementos. Usa un algoritmo rápido que tarda la mitad del tiempo, para
    // el caso particular en que M=N+1, o sea, cuando el tamaño de muestreo
    // del contorno (M) es igual al tamaño de  muestreo del CPS (N) aumentado
    // en una unidad.
    const double area = sqrt(contourArea(newSample));
    return cps.full_cps(newSample, 128, 127, area);
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
    for(int i = 0; i <  contours.size(); i++){
        cvCircle(img, cvPoint(contours[i][0].x,contours[i][0].y),1,CV_RGB(250,244,227),3,1,1);
    }


    cvShowImage("Image",img);
    cvWaitKey( 0 );
};


#endif //CPSWITHSPLINES_MAIN_H
