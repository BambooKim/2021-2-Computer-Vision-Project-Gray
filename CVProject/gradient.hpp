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

extern int** xConvolvMat;
extern int** yConvolvMat;

void showImage(String srcName, Mat img);
void gradXFilter(Mat* mat, int width, int height);
void gradYFilter(Mat* mat, int width, int height);
void gradFilter(Mat* mat, int width, int height);

#endif /* gradient_hpp */
