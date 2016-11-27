//
// Created by Santos on 11/27/2016.
//


#ifndef CPSWITHSPLINES_CPSFUNCTIONS_H
#define CPSWITHSPLINES_CPSFUNCTIONS_H

#include "main.hpp"
#include "contourUtilities.hpp"

/*Functions prototype declaration*/
MatrixXd generateCpsWithSplineRefinement(std::vector<cv::Point> X, const double area);
/*Functions implementation*/
MatrixXd computeCps(std::vector<cv::Point> contourPoints, const double area);
//only for debug
std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);
cv::Point2d matchingCps(cvx::CpsMatrix cpsA, cvx::CpsMatrix cpsB);


/**
 * This method is used to generate the cps matrix, using the cubic spline function constructed with the countour points.
 * TODO: Implement a class with this functions and attributes.
 */
MatrixXd generateCpsWithSplineRefinement(std::vector<cv::Point> vector, const double area) {

    // Define aux arraysfac

    int points = vector.size();

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


    cv::Mat img =  generateSplineBasedFigure( resultsMatrixX,  resultsMatrixY, 1000, 1000);
    std::stringstream s;
    s << "sample_" << std::rand();
    cv::imshow(s.str(),img);

    cvWaitKey(0);
    std::vector<cv::Point> pointsFromSpline = samplePointsFromSpline(resultsMatrixX, resultsMatrixY, points);
    //printNewSample(pointsFromSpline);
    return computeCps(pointsFromSpline, area);
}



/**
 * Create the cps signature for a specific contour.
 */
MatrixXd computeCps(std::vector<cv::Point> contourPoints, const double area) {
    MatrixXd cps(contourPoints.size(),contourPoints.size());
    MatrixXd aux(contourPoints.size(),contourPoints.size());
    /*"initialize with "0"*/
    for(int i=0 ; i < contourPoints.size() ; i++ ){
        for(int j=0 ; j < contourPoints.size() ; j++ ){
            aux(i,j) = 0;
        }
    }
    /* Calculate the distance between points */
    for(int i=0 ; i < contourPoints.size() ; i++ ){
        for(int j=0 ; j < contourPoints.size() ; j++ ){
            if(i<j){
                aux(i,j) = sqrt((contourPoints[i].x - contourPoints[j].x)*(contourPoints[i].x - contourPoints[j].x) + (contourPoints[i].y - contourPoints[j].y)*(contourPoints[i].y - contourPoints[j].y));
                if(aux(i,j)==0){
                    aux(i,j) = 1;
                }
            } else if(i>j){
                aux(i,j) = aux(j,i);
            }
        }
    }
    /* we need to rotate the rows i-1 places to the left  and we normalize the values*/
    for(int i=0 ; i < contourPoints.size() ; i++ ){
        for(int j=0 ; j < contourPoints.size() ; j++ ){
            if(j+1+i>= contourPoints.size()){
                cps(i,j) = aux(i,j+1+i - contourPoints.size())/sqrt(area);
            }else{
                cps(i,j) = aux(i,j+1+i)/sqrt(area);
            }
        }
    }

    return cps;

}


/**
 * This method get the distance between two cps matrix.
 */
std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb) {
    std::vector<double> result;
    /* Number of point samples*/
    double n = mta.rows();
    MatrixXd matrix((int)n,(int)n);

    /*Each value of k represent a different rotation*/
    for(int k = 0; k <  n; k++) {
        std::vector<int> vector;
        for (double u = k - 1; u < n-1 + k; u++) {
            if(u >= n-1)
                vector.push_back((int)fmod( u , n-1));
            else
                vector.push_back((int)fmod( u , n-1)+1);

        }
        /*Calculate the euclidian distance*/
        for(int i = 0; i <  n; i++) {
            double sumDist = 0;
            for(int j = 0; j < n; j++) {
                sumDist += (mta(i,j)-mtb(vector[i],j)) * (mta(i,j)-mtb(vector[i],j));
            }
            matrix(i,k) = sqrt(sumDist);
        }
    }
    /*the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = minSum(matrix);
    result.push_back(matchingData.y);
    double maxValue = matrix(0,matchingData.y);
    for(int i = 0; i <  n; i++) {
        if(matrix(i,matchingData.y)>maxValue){
            maxValue = matrix(i,matchingData.y);
        }
    }
    result.push_back(maxValue );
    result.push_back((matchingData.x/mta.rows()) );

    return result;
}


/**
* This method is going to make the matching step, using the euclidian distance.(is possible to use the r_measure distance measure).
*/
cv::Point2d matchingCps(cvx::CpsMatrix cpsA, cvx::CpsMatrix cpsB){
    /* Number of point samples*/
    double n = cpsA.data().rows;
    MatrixXd matrix;


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
            matrix(i,k) = sqrt(sumDist);
        }
        vectorIndex = 0;
    }
    /* the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = minSum(matrix);
    return matchingData;
}


#endif //CPSWITHSPLINES_CPSFUNCTIONS_H
