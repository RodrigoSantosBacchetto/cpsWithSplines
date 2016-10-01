
#include "main.hpp"

double afinTransformation(double aX[], double yX[], double bX[], double bY[], int sampleSize, double pi);

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
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = image.clone();
    cv::Mat mathEdge;
    cv::threshold(contourOutput, mathEdge, 192.0, 255.0, CV_THRESH_BINARY_INV);
    cv::findContours( contourOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    int points = sample;

    double X[points];
    double Y[points];

    /*Extract points into arrays*/
    convertPoints(X, Y, contours, sample);

    cvx::CpsMatrix mta = generateCpsWithSplineRefinement(points, X, Y, contours);
    cvx::CpsMatrix mtb = generateCpsWithSplineRefinement(points, X, Y, contours);

    /* TODO: struct of this.
     * the X(METRIC 1) coordinate is the minim sum and the Y coordinate is the index of that column on the matrix cpsA*/
    cv::Point2d matchingData = matchingCps( mta, mtb);
    /* Result distance of point matching*/
    double dist1 = matchingData.x;
    /* Column index with the minor value*/
    double pi = matchingData.y;
    /* Result distance of afin transformation*/
    double dist2 = afinTransformation( X, Y, X, Y, sample, pi);

    double finalDisimilarityDistance = dist1*alfa + beta*dist2;
}

/**
 * This method get the
 */
double afinTransformation(double aX[], double aY[], double bX[], double bY[], int sampleSize, double pi) {
    Matrix2d mat;
    Matrix2d mat2;
    Matrix2d t = mat * mat2;
    mat = t.inverse();

    return 2.2;
}

/**
 * This matlab code implements the AFIN transformation.
 */
/*% TTRANSFORMACION AFIN
% y=T(x)=Ax+sigma

%References
%_(Belongie 2002 IEEE) Shape Matching and Object Recognition Using Shape Contexts

function distancia=transf_afin(DatosA,DatosB,N,d_method)

%Se transforma el sistema de coordenadas
DatosA{1}=transf_coord(DatosA{1},1);
DatosB{1}=transf_coord(DatosB{1},1);

prefA=selec_puntos(N,DatosA{4},DatosA{1});
prefB=selec_puntos(N,DatosB{4},DatosB{1});

F=cps(prefA,DatosA{2});
G=cps(prefB,DatosB{2});

[pi,min_sum]=matching_cps(F,G,d_method);
%pi=N-pi; %Correcion por cambio de coordenadas

prefB=[prefB(pi:N,:);prefB(1:pi-1,:)];

P=[prefA' ;ones(1,N)];
Q=[prefB' ;ones(1,N)];

%sigma=mean(P-Q);

% Moore-Penrose pseudoinverse of matrix
%psQ=pinv(Q);
psP=pinv(P);

%A=(psQ*P)';
%A(1:2,1:2)=A(1:2,1:2)';
%A
A=Q*psP;

%A=A(end:-1:1,end:-1:1);
M=size(DatosA{1},1);
for i=1:M
contourA_T=A*[ DatosA{1}' ; ones(1,M)];
end
contourA_T=contourA_T(1:2,:)';

%subplot(1,2,1)
%%contour2image(DatosA{1},[255 0 0]);
%scatter(DatosA{1}(:,1),DatosA{1}(:,2),'xr')
%subplot(1,2,2)
%hold on
%%contour2image(DatosB{1},[0 0 255]);
%scatter(DatosB{1}(:,1),DatosB{1}(:,2))
%%subplot(2,2,3)
%%contour2image(round(contourA_T),[255 0 0]);
%scatter(contourA_T(:,1),contourA_T(:,2),'xr')

%%subplot(2,2,4)
%%contour2image([DatosB{1} ; round(contourA_T)],[0 255 0]);

distancia=sum(sum(abs(A*P-Q)));

end*/



