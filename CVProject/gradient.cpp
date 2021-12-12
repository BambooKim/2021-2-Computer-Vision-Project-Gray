//
//  gradient.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/10.
//

#include "gradient.hpp"

//#define threshold
#define none
#define PRODUCT 2

using namespace cv;
using namespace std;

double** xConvolvMat;
double** yConvolvMat;
double** magnitudes;

// x방향의 gradient를 구하는 함수.
void gradXFilter(Mat* mat) {
    Mat src;
    (*mat).copyTo(src);
    
    int width = src.size().width;
    int height = src.size().height;
    
    // x convolution filter
    // gradient
    int x_kernel[3] = { 1, 0, -1 };
       
    int min = 255;
    int max = 0;
    
    for (int y = 1; y < height - 1 ; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // x 방향의 부분 픽셀값.
            int x_img[3];
            for (int i = -1; i < 2; i++) {
                x_img[i + 1] = (int) src.at<uchar>(x + i, y);
            }
            
            // x kernel과 x 방향의 부분 픽셀값 convolution.
            int x_gra = 0;
            for (int i = 0; i < 3; i++) {
                x_gra = x_gra + x_img[i] * x_kernel[i];
            }
        
            // Mat 구조체에 저장하면 음수는 자동으로 값이 양수로 변하므로
            // xConvolvMat에 x방향의 gradient 값을 저장한다.
            xConvolvMat[y][x] = x_gra;
            
            if (min > x_gra)
                min = x_gra;
            if (max < x_gra)
                max = x_gra;
        }
    }
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // 시각화를 위해 음수를 양수로 만들어주기 위해
            // 그만큼 더해준 뒤 폭의 scaling을 위해
            // 절반으로 나눠준다.
            int absMin = min < 0 ? -min : min;

#ifdef none
            int x_gra = (xConvolvMat[y][x] + absMin) * PRODUCT;
#endif
            int in = x_gra;
        
            // Mat 구조체에 보정된 gradient 값을 저장한다.
            src.at<uchar>(x, y) = in;
        }
    }
}

// y 방향의 gradient를 구하고 화면에 보여주는 함수.
void gradYFilter(Mat* mat) {
    Mat src;
    (*mat).copyTo(src);
    
    int width = src.size().width;
    int height = src.size().height;
    
    // y 방향의 linear derivative filter
    int y_kernel[3] = { 1, 0, -1 };
    
    int min = 255;
    int max = 0;
    
    for (int y = 1; y < height - 1 ; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // y 방향의 partial image의 픽셀값
            int y_img[3];
            for (int i = -1; i < 2; i++) {
                y_img[i + 1] = (int) src.at<uchar>(x, y + i);
            }
            
            // y방향의 partial image와 kernel의 convolution
            int y_gra = 0;
            for (int i = 0; i < 3; i++) {
                y_gra = y_gra + y_img[i] * y_kernel[i];
            }
        
            // 역시 mat 구조체에 저장할 경우 음수가 양수가 되기 때문에
            // yConvolvMat 2차원 배열에 gradient 값을 저장한다.
            yConvolvMat[y][x] = y_gra;
            
            if (min > y_gra)
                min = y_gra;
            if (max < y_gra)
                max = y_gra;
        }
    }
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // 역시 음수를 양수로 만들고 2로 나눠
            // gradient 값을 보정한다.
            int absMin = min < 0 ? -min : min;

#ifdef none
            int y_gra = (yConvolvMat[y][x] + absMin) * PRODUCT;
#endif
            int in = y_gra;

            src.at<uchar>(x, y) = in;
        }
    }
}

// x방향과 y방향의 gradient를 가지고 magnitude를 구한다.
Mat gradFilter(Mat* mat) {
    gradXFilter(mat);
    gradYFilter(mat);
    // 위 두 함수가 수행되면 전역 변수에 gradient가 저장된다.
    
    Mat src = (*mat).clone();
    int width = src.size().width;
    int height = src.size().height;
    
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            // x와 y의 gradient를 이용해 magnitude를 구한다.
            double magnitude = sqrt(xConvolvMat[y][x] * xConvolvMat[y][x] + yConvolvMat[y][x] * yConvolvMat[y][x]);
#ifdef dog
            src.at<uchar>(x, y) = magnitude * PRODUCT;
#endif
#ifdef none
            src.at<uchar>(x, y) = magnitude;
#endif
#ifdef threshold
            src.at<uchar>(x, y) = magnitude > 60 ? 255 : 0;
#endif
            
            // 전역 변수 2차원 배열 magnitudes에 magnitude값을 저장한다.
            magnitudes[y][x] = magnitude;
        }
    }
    
    return src;
}
