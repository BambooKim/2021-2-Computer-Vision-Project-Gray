//
//  derivOfGaussian.hpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#ifndef derivOfGaussian_hpp
#define derivOfGaussian_hpp

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double derivativeGaussian(int n, double sigma);
double* createDoGKernel(int size, double sigma);
void DoG(Mat* mat, int kernelSize, double sigma, int width, int height);

#endif /* derivOfGaussian_hpp */
