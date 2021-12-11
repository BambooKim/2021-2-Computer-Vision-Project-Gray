//
//  gradient.hpp
//  CVProject
//
//  Created by 김범구 on 2021/12/10.
//

#ifndef gradient_hpp
#define gradient_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

extern double** xConvolvMat;
extern double** yConvolvMat;
extern double** magnitudes;

void gradXFilter(Mat* mat);
void gradYFilter(Mat* mat);
Mat gradFilter(Mat* mat);

#endif /* gradient_hpp */
