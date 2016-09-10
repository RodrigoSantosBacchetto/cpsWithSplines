
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

void drawNow(MatrixXd resultsMatrixX, MatrixXd resultsMatrixY, std::vector<std::vector<cv::Point>> vector);

int main() {

    int sample = 20;


    cv::Mat image= cv::imread("C:\\Users\\Santos\\Desktop\\spoon-4.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }



/*    cvx::PlanarSet shape;*/
/*    shape.imread("C:\\Users\\Santos\\Desktop\\spoon-4.jpg");*/
    // Obtiene los contornos de la silueta.
/*    cvx::Boundary sil = shape.boundary();*/
    // Crea la función de contorno, usando el contorno externo (índice=0).
/*    cvx::Curve con = sil.largest();*/
    // Crea la función que computa el CPS.
/*    cvx::CpsEuclidean cps;*/
    // Crea dos matrices de CPS. Cada fila es el CPS de un punto sampleado.
/*    cvx::CpsMatrix mta, mtb;*/
    // Obtiene el CPS de M=128 puntos, con una definición de M=127 muestras
    // (se elimina la columna con ceros). La matriz resultante es de 128x127
    // elementos. Usa un algoritmo rápido que tarda la mitad del tiempo, para
    // el caso particular en que M=N+1, o sea, cuando el tamaño de muestreo
    // del contorno (M) es igual al tamaño de  muestreo del CPS (N) aumentado
    // en una unidad.
 /*   mta = cps(con, 128);*/



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
    if(eqSolver.info() != Success){
        // Matrix decomposition failed!
        std::cout << "There was an error de-composing the matrix" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
    }

    XResults = eqSolver.solve(XRightHand);
    if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for X" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
    }

    YResults = eqSolver.solve(YRightHand);
    if(eqSolver.info() != Success){
        // Equation solver failed
        std::cout << "There was an error solving the linear system for Y" << std::endl << eqSolver.info();
        //XResults << -9999;
        return -1;
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


    drawNow(resultsMatrixX,
            resultsMatrixY,
            contours);

    std::cout << "X - Final Matrix:" << std::endl << resultsMatrixX << std::endl;
    std::cout << "Y - Final Matrix:" << std::endl << resultsMatrixY << std::endl;

    std::cout << std::endl << resultsMatrixX(0,0) + resultsMatrixX(0,1) + resultsMatrixX(0,2) + resultsMatrixX(0,3);

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