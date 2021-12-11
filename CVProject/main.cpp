//
//  main.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/09.
//

#include <iostream>
#include "gradient.hpp"
#include "gaussian.hpp"
#include "derivOfGaussian.hpp"
#include "canny.hpp"

using namespace std;
using namespace cv;

extern double** xConvolvMat;
extern double** yConvolvMat;
extern double** magnitudes;

void initGlobalArray(int width, int height) {
    xConvolvMat = new double*[height];
    yConvolvMat = new double*[height];
    magnitudes = new double*[height];
    for (int i = 0; i < height; i++) {
        xConvolvMat[i] = new double[width];
        yConvolvMat[i] = new double[width];
        magnitudes[i] = new double[width];
    }
}

void deleteGlobalArray(int height) {
    for (int i = 0; i < height; i++) {
        delete[] xConvolvMat[i];
        delete[] yConvolvMat[i];
        delete[] magnitudes[i];
    }
    delete[] xConvolvMat;
    delete[] yConvolvMat;
    delete[] magnitudes;
}

int main(int argc, const char * argv[]) {
    // 이미지 파일을 회색으로 읽어들인다. 해당 정보는 OpenCV의 Mat 구조체에 저장된다.
    string fileName;
    //cin >> fileName;
    fileName = "IMG_5473.jpeg";
    String filePath = "/Users/bambookim/Desktop/CVProject/CVProject/" + fileName;
    Mat src = imread(filePath, IMREAD_GRAYSCALE);

    // 이미지의 가로, 세로, 채널 정보를 얻는다.
    int width = src.size().width;
    int height = src.size().height;
    int channel = src.channels();
    
    // 전역 변수 2차원 배열 xConvolvMat와 yConvolvMat를 동적 할당한다.
    initGlobalArray(width, height);
    
    imshow("Source", src);
    waitKey();
    
    Mat CannyImg = src.clone();
    
    CannyImg = gaussianFilter(&CannyImg, 15, 2);
    imshow("Gaussian", CannyImg);
    waitKey();
    
    CannyImg = gradFilter(&CannyImg);
    imshow("Gaussian-Derivative", CannyImg);
    waitKey();
    
    Mat CannyDouble = CannyImg.clone();
    CannyDouble = cannyEdgeDouble(&CannyDouble);
    imshow("Canny Edge Double", CannyDouble);
    waitKey();
    
    CannyImg = cannyEdgeTriple(&CannyImg);
    imshow("Canny Edge Triple", CannyImg);
    waitKey();
    
    // 동적 할당을 해제한다.
    deleteGlobalArray(height);
    
    return 0;
}
