
#include "main.hpp"

#define MINUS_ONE 99
#define DOS 150

//void getSortedContour(cv::Mat);
//void getKuimContour (cv::Mat, int);
//int getNext(int x, int y, int last, cv::Mat data, int totalRows, int totalCols);
//
//int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
//int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

int main(){

    cv::Mat image= cv::imread("D:\\FP-UNA\\Image Databases\\spoon-1.jpg",0);
    if (!image.data) {
        std::cout << "Image file not found\n";
        return 1;
    }

    getKuimContour(image,1);
    cvWaitKey( 0 );
}

std::vector<cv::Point> getKuimContour (cv::Mat originalImage, int numberOfContours) {

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
//
//    for( int m=0; m < data1.rows; m++){
//        std::cout << "m: " << m << " ";
//        for( int n=0; n < data1.cols; n++){
//            std::cout << (int)(data2.at<uchar>(m,n)) << " -- ";
//        }
//        std::cout << std::endl;
//    }
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

void getSortedContour(cv::Mat originalImage) {

    //clone the image to not alter the original one
    cv::Mat clonedImg = originalImage.clone();
    cv::imshow("OriginalImage", clonedImg);

    int totalRows = clonedImg.rows;
    int totalCols = clonedImg.cols;

    cv::Mat binImage;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    //turn image into binary
    cv::threshold(clonedImg, binImage, 192.0, 255.0, CV_THRESH_BINARY);
    cv::imshow("BinaryImage", binImage);

    //find the external contour of the binary image
    cv::findContours(clonedImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //how many points in the contour?
    int totalPointsOnContour = 0;
    for (int i = 0; i < contours.size(); i++)
        totalPointsOnContour += contours[i].size();

    IplImage *img = cvCreateImage(cvSize(totalRows, totalCols), 8, 1);
    CvPoint pointsToDraw[totalPointsOnContour];

    int index = 0;
    for (int i = 0; i < contours.size(); i++) {
        for (int j = 0; j < contours[i].size(); j++) {
            //pointsToDraw[index] = cvPoint(contours[i][j].x, contours[i][j].y);
            std::cout << "X: " << contours[i][j].x << " -- Y: " << contours[i][j].y << std::endl;
            pointsToDraw[index] = contours[i][j];
            index++;
        }
    }

    for (int j = 0; j <= index; j++) {
        cvCircle(img, pointsToDraw[j], 1, CV_RGB(255, 0, 0), 3, 1, 1);
    }

    cv::Mat contourImg = cv::cvarrToMat(img);
    imshow("DrawnContour", contourImg);

    // okay... asumming the matrix got built correctly

    //find starting point

};
