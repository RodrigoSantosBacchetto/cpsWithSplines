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
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <map>
#include "dirent.h"

#define MINUS_ONE 99
#define DOS 150
#define ONLY_EXTERNAL_CONTOUR 1

using namespace Eigen;

typedef Eigen::SparseMatrix<double> sMatrix;


int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

#endif //CPSWITHSPLINES_MAIN_H
