//
// Created by Santos on 11/27/2016.
//

#ifndef CPSWITHSPLINES_GENERALFUNCTIONS_H
#define CPSWITHSPLINES_GENERALFUNCTIONS_H

#include "main.hpp"

cv::Point2d minSum(cv::Mat mat);
int getNext(int x, int y, int last, cv::Mat data, int totalRows, int totalCols);
double getMaxMinValue(std::vector<double> vector,std::string valueType);


/**
 *
 * This method is used to get the column that has the minum sum value(saved in X) and the index of that column(saved on Y).
 * X is the result value of the metric dist_1.
 */
cv::Point2d minSum(MatrixXd mat) {
    /*We get the sum of the values for each column*/
    std::vector<double> vector;
    for(double j = 0; j <  mat.cols(); j++) {
        double sum = 0 ;
        for (double i = 0 ; i < mat.rows() ; i++) {
            sum += mat(i,j);
        }
        vector.push_back(sum);
    }
    /*We get the minum value in the vector and the index of that value which represents the column of the matrix with the minum value*/
    double min = vector[0];
    int index = 0;
    for(int j = 0; j <  vector.size(); j++) {
        // 10 is a percentage that indicates, that we are going to consider only 0.2 percent of the rotations
        if(vector[j]<min and (j<=( vector.size()/6) or j>=((vector.size()/6)*5))){
            min = vector[j];
            index = j;
        }
    }
    cv::Point2d result;
    result.x = min;
    result.y = index;
    return result;
}


int getNext(int x, int y, int last, cv::Mat data, int totalRows, int totalCols) {
    int ctrl = 1;
    int next = (last + 2) % 8;

    int nx = x + dx[next];
    int ny = y + dy[next];

    while((ctrl<8) && ((nx < 0) || (nx > totalCols) || (ny < 0) || (ny > totalRows) || ((int)(data.at<uchar>(ny,nx)) == 0))) {
        next = (next + 1) % 8;
        nx = x + dx[next];
        ny = y + dy[next];
        ctrl++;
    }

    return next;
}


/**
 * This method get the max or min value on a vector of doubles.
 */
double getMaxMinValue(std::vector<double> vector, std::string valueType) {
    double tempVal = 0;
    for(int i = 0; i < vector.size(); i++){
        tempVal = tempVal + vector[i];
    }
    return tempVal/vector.size();
}

template<typename _Matrix_Type_>
_Matrix_Type_ pseudoInverse(const _Matrix_Type_ &a, double epsilon = std::numeric_limits<double>::epsilon())
{
    Eigen::JacobiSVD< _Matrix_Type_ > svd(a ,Eigen::ComputeThinU | Eigen::ComputeThinV);
    double tolerance = epsilon * std::max(a.cols(), a.rows()) *svd.singularValues().array().abs()(0);
    return svd.matrixV() *  (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal() * svd.matrixU().adjoint();
}


#endif //CPSWITHSPLINES_GENERALFUNCTIONS_H
