//
// Created by Gerardo :D
//



#ifndef CPSWITHSPLINES_NOISEFUNCTIONS_H
#define CPSWITHSPLINES_NOISEFUNCTIONS_H

#include "main.hpp"

typedef struct noise_output {
    std::vector<cv::Point> noisyContour;
    double noisy_area;
} noise_output;

noise_output addNoiseToContour(std::vector<cv::Point> originalContour, double SNR_db);
std::vector<cv::Point> getNoisyContour(std::vector<cv::Point> originalContour, double SNR_db);

noise_output addNoiseToContour(std::vector<cv::Point> originalContour, double SNR_db) {
    noise_output noisyOutput;

    // Calculate SNR
    double SNR = pow(10.0, (SNR_db/10.0));
    //std::cout << std::endl << "snr:" << SNR;

    // Calculate the contour's centroide - and the distances between any pair of consecutive points
    double centroide_x;
    double centroide_y;
    std::vector<double> a;
    double current_a;
    double twice_area = 0;
    double ux = 0;
    double uy = 0;

    for (int k = 1; k < originalContour.size(); k++){

        //centroide computation
        current_a = (originalContour[k-1].x * originalContour[k].y) - (originalContour[k-1].y * originalContour[k].x);
        twice_area += current_a;
        a.push_back(current_a);

        ux += (((double)originalContour[k-1].x + (double)originalContour[k].x) * current_a);
        uy += (((double)originalContour[k-1].y + (double)originalContour[k].y) * current_a);

    }
    //std::cout << std::endl << "AREA:" << (0.5 * twice_area);

    centroide_x = (ux/6) / (0.5 * twice_area);
    centroide_y = (uy/6) / (0.5 * twice_area); //centroide... completed!
    //std::cout << std::endl << "CENTROIDE: [" << centroide_x << ", " << centroide_y << "]";

    // determine mean distance between all contour points and the centroide -- this is in order to compute the standard deviation
    double umean = 0;
    for (int k = 0; k < originalContour.size(); k++) {
        double current_computation = pow((double)originalContour[k].x - centroide_x, 2.0);
        current_computation += pow((double)originalContour[k].y - centroide_y, 2.0);
        current_computation = sqrt(current_computation);

        umean += current_computation;
    }
    umean = umean / originalContour.size();

    double std_dev = umean/SNR;
    //std::cout << std::endl << "STD DEV:" << std_dev;


    // effectively add the noise to the contour
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,std_dev);

    for (int i = 0; i < originalContour.size(); i++) {

        int dx = (int)round(distribution(generator));
        int dy = (int)round(distribution(generator));
        //std::cout << std::endl << "rndval: " << dx << " -- " << dy ;
        int xval = originalContour[i].x + dx;
        int yval = originalContour[i].y + dy;

        noisyOutput.noisyContour.push_back(cvPoint(xval, yval));
    }

    noisyOutput.noisy_area = 0.5 * twice_area;
    noisyOutput.noisy_area = noisyOutput.noisy_area < 0 ? (-1)*noisyOutput.noisy_area : noisyOutput.noisy_area;

    return noisyOutput;
}


std::vector<cv::Point> getNoisyContour(std::vector<cv::Point> originalContour, double SNR_db){
    noise_output r = addNoiseToContour(originalContour, SNR_db);
    return r.noisyContour;
}

#endif //CPSWITHSPLINES_NOISEFUNCTIONS_H