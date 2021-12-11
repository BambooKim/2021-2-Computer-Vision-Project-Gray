//
//  canny.hpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#ifndef canny_hpp
#define canny_hpp

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

extern double** xConvolvMat;
extern double** yConvolvMat;
extern double** magnitudes;

Mat cannyEdgeDouble(Mat* mat);
Mat cannyEdgeTriple(Mat* mat);
Mat nonMaxSuppress(Mat* mat);

#endif /* canny_hpp */
