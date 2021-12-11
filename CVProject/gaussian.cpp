//
//  gaussian.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#include "gaussian.hpp"
#include "gradient.hpp"

// 주어진 입력 n에 대하여 표준편차 sigma를 가지는
// 가우시안 분포의 값을 반환한다.
double _1D_Gaussian(int n, double sigma) {
    double gaussian = exp(-1 * (n * n) / (2 * sigma * sigma));
    
    return gaussian;
}

// _1D_Gaussian() 함수를 이용하여
// 주어진 커널 사이즈와 표준편차의 값을 가지는
// 가우시안 커널(double pointer)을 반환한다.
double* create1DGaussianKernel(int size, double sigma) {
    double* kernel = new double[size];
    for (int i = -1 * (size / 2); i < size / 2 + 1; i++) {
        kernel[i + size / 2] = _1D_Gaussian(i, sigma);
    }
    
    return kernel;
}

// 주어진 이미지와 커널 사이즈, 표준편차 sigma를 이용하여
// Gaussian Smoothing된 이미지를 화면에 보여준다.
//
// Convolution의 특성을 이용하여 x 방향으로 가우시안 합성을 한 뒤
// 중간 계산값을 이용하여 y 방향으로 한번 더 가우시안 합성을 한다.
Mat gaussianFilter(Mat* mat, int kernelSize, double sigma) {
    Mat src = (*mat).clone();
    int size = kernelSize;
    
    int width = src.size().width;
    int height = src.size().height;
    
    // kernel size로 size를, 표준편차로 sigma를 가지는 가우시안 커널을 얻는다.
    // 이때 배열의 크기는 size이다.
    double* kernel = create1DGaussianKernel(size, sigma);
    
    // 최종 convolution 계산값... scale 보정 전의 값.
    double** convolvMat = new double*[height];
    // x방향 convolution 계산값을 저장할 2차원 배열
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
            
            // 가우시안 커널과 이미지를 가지고 x방향 convolution
            for (int i = 0; i < size; i++) {
                if (0 <= x + i - size / 2 && x + i - size / 2 <= width) {
                    temp = temp + kernel[i] * (double) src.at<uchar>(x + i - size / 2, y);
                } else {
                    temp = 0;
                }
            }
            
            if (min > temp)
                min = temp;
            if (max < temp)
                max = temp;
            
            _xConvolvMat[y][x] = temp;
        }
    }
    
    long double MAX = max;
    min = 1.79E+308;
    max = 0;
    
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double temp = 0;
            
            // x convolution의 최댓값을 이용하여 scale을 보정한다.
            // 보정한 값과 커널을 이용하여 y 방향 convolution.
            for (int i = 0; i < size; i++) {
                int num = y + i - size / 2;
                if (!(0 <= num && num < height)) {
                    num = 0;
                }
                
                double data = _xConvolvMat[num][x];
                
                data = data / (MAX / 255.0 + 1);
                temp = temp + kernel[i] * data;
            }
            
            if (min > temp)
                min = temp;
            if (max < temp)
                max = temp;
            
            convolvMat[y][x] = temp;
        }
    }
    
    // 최종 convolution의 최댓값을 이용하여 scale을 보정한다.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double data = convolvMat[y][x];
            data = data / (max / 255.0 + 1);
            
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
    
    return src;
}
