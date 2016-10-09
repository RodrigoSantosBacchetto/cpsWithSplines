
#include "main.hpp"



std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb);

int main() {

    double alfa = 1;
    double beta = 1;
    int sample = 20;


    cv::Mat image= cv::imread("C:\\Users\\Santos\\Desktop\\spoon-4.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<cv::Point> fullContour = getKuimContour(image, ONLY_EXTERNAL_CONTOUR);
    std::vector<cv::Point> sampledPoints = sampleContourPoints(fullContour, sample);

    MatrixXd mta = generateCpsWithSplineRefinement(sampledPoints);
    MatrixXd mtb = generateCpsWithSplineRefinement(sampledPoints);

    /* the first value is de column index with the minimu sum*/
    /* the second value is the max distance between two matrix*/
    /* the third value is the promedian distance between two matrix*/
    std::vector<double> dist = smCpsRm( mta, mtb);
}



/**
 * This method get the distance between two cps matrix.
 */
std::vector<double> smCpsRm(MatrixXd mta, MatrixXd mtb) {
    std::vector<double> result;
    /* Number of point samples*/
    double n = mta.rows();
    std::vector< std::vector<double> > matrix((unsigned long long int)n, std::vector<double>((unsigned long long int)n));


     /*Each value of k represent a different rotation*/
    for(int k = 0; k <  n; k++) {
        std::vector<int> vector;
        for (double u = k - 1; u < n + k; u++) {
            vector.push_back((int)fmod( u , n)+1);

        }
         /*Calculate the euclidian distance*/
        for(int i = 0; i <  n; i++) {
            double sumDist = 0;
            for(int j = 0; j <  n; j++) {
                sumDist += pow(mta(i,j)-mtb(vector[i],j),2);
            }
            matrix[i][k] = sqrt(sumDist);
        }
    }
     /*the X(METRIC 1) coordenate is the minim sum and the Y cordenate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = minSum(matrix);
    result.push_back(matchingData.y);
    result.push_back(matchingData.x/mta.rows());
    double minValue = matrix[0][matchingData.y];
    for(int i = 0; i <  n; i++) {
        if(matrix[i][matchingData.y]<minValue){
            minValue = matrix[i][matchingData.y];
        }
    }
    result.push_back(minValue);

    return result;
}




