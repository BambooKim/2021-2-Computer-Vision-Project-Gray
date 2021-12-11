//
//  gaussian.hpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#ifndef gaussian_hpp
#define gaussian_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

double _1D_Gaussian(int n, float sigma);
double* create1DGaussianKernel(int size, float sigma);
Mat gaussianFilter(Mat* mat, int kernelSize, double sigma);

#endif /* gaussian_hpp */
