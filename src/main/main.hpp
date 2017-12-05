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
#include <Eigen/Core>
#include <Eigen/SVD>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include "dirent.h"

#define MINUS_ONE 99
#define DOS 150
#define ONLY_EXTERNAL_CONTOUR 1

using namespace Eigen;

typedef Eigen::SparseMatrix<double> sMatrix;

typedef struct {
    MatrixXd CPSMatrix;
    std::vector<cv::Point> pointSample;
} cspResult;
typedef struct {
    std::string imagePath;
    cv::Mat image;
} dualImage;

int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};


typedef struct classResults {
    std::string class_name;
    std::vector<dualImage> images;

    std::vector<cspResult> cp_signatures_16; // n
    std::vector<cv::Point> prfA; // n
    std::vector<double> same_class_distances_16; // (n-1) / 2
    std::vector<std::string> same_class_path_16; // (n-1) / 2
    std::vector<double> diff_class_distances_16; // (n-1) / 2
    std::vector<std::string> diff_class_path_16; // (n-1) / 2
    std::double_t max_value_16= 0;
    std::double_t error_value_16= 0;

    std::vector<cspResult> cp_signatures_32; // n
    std::vector<double> same_class_distances_32; // (n-1) / 2
    std::vector<double> diff_class_distances_32; // (n-1) / 2
    std::double_t max_value_32= 0;
    std::double_t error_value_32= 0;

    std::vector<cspResult> cp_signatures_64;
    std::vector<double> same_class_distances_64;
    std::vector<double> diff_class_distances_64;
    std::double_t max_value_64= 0;
    std::double_t error_value_64= 0;

    std::vector<cspResult> cp_signatures_86;
    std::vector<double> same_class_distances_86;
    std::vector<double> diff_class_distances_86;
    std::double_t execution_time_86= 0;

    std::vector<cspResult> cp_signatures_128;
    std::vector<double> same_class_distances_128;
    std::vector<double> diff_class_distances_128;
    std::double_t execution_time_128= 0;

    std::vector<cspResult> cp_signatures_256;
    std::vector<double> same_class_distances_256;
    std::vector<double> diff_class_distances_256;
    std::double_t execution_time_256 = 0;

} classResults;



#endif //CPSWITHSPLINES_MAIN_H
