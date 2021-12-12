//
//  derivOfGaussian.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#include "derivOfGaussian.hpp"
#include <cmath>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;

// 가우시안 분포를 1차 미분한 값을 반환한다.
double derivativeGaussian(int n, double sigma) {
    double gaussian = -1 * n * (exp(-1 * (n * n) / (2 * sigma * sigma)) / (sigma * sigma));
    
    return gaussian;
}

// derivativeGaussian() 함수로부터 얻은 값을 바탕으로 DoG 커널을 만든다.
double* createDoGKernel(int size, double sigma) {
    double* kernel = new double[size];
    for (int i = -1 * (size / 2); i < size / 2 + 1; i++) {
        kernel[i + size / 2] = derivativeGaussian(i, sigma);
    }
    
    return kernel;
}

void DoG(Mat* mat, int kernelSize, double sigma, int width, int height) {
    Mat src = (*mat);
    int size = kernelSize;
    
    // DoG Kernel
    double* kernel = createDoGKernel(size, sigma);
     
    double** convolvMat = new double*[height];
    double** _xConvolvMat = new double*[height];
    
    for (int i = 0; i < height; i++) {
        convolvMat[i] = new double[width];
        _xConvolvMat[i] = new double[width];
    }
    
    long double min = 1.79E+308;
    long double max = 0;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double temp = 0;
            
            // DoG 커널과 이미지를 가지고 x방향 convolution
            for (int i = 0; i < size; i++) {
                temp = temp + kernel[i] * (double) src.at<uchar>(x + i - size / 2, y);
            }
            
            if (min > temp)
                min = temp;
            if (max < temp)
                max = temp;
            
            _xConvolvMat[y][x] = temp;
        }
    }
    
    long double MAX = max;
    long double MIN = min;
    min = 1.79E+308;
    max = 0;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double temp = 0;
            
            for (int i = 0; i < size; i++) {
                int num = y + i - size / 2;
                // 인덱스 범위 안에 있는지 확인.
                if (!(0 <= num && num < height)) {
                    num = 0;
                }
                
                double data = _xConvolvMat[num][x];
                
                // 스케일 보정.
                double absMin = MIN < 0 ? -MIN : 0;
                data = (data + absMin) / ((absMin + MAX) / 255.0);
                
                // y 방향 convolution.
                temp = temp + kernel[i] * data;
            }
            
            if (min > temp)
                min = temp;
            if (max < temp)
                max = temp;
            
            convolvMat[y][x] = temp;
        }
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double data = convolvMat[y][x];
            
            double absMin = min < 0 ? -min : 0;
            
            // scale 보정.
            data = (data + absMin) / ((max + absMin) / 255.0);
            
            src.at<uchar>(x, y) = data;
        }
    }
    
    delete[] kernel;
    for (int i = 0; i < height; i++) {
        delete[] _xConvolvMat[i];
        delete[] convolvMat[i];
    }
    delete[] _xConvolvMat;
    delete[] convolvMat;
}
