//
//  gradient.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/10.
//

#include "gradient.hpp"

using namespace cv;
using namespace std;

int** xConvolvMat;
int** yConvolvMat;

void showImage(String srcName, Mat img) {
    imshow(srcName, img);
    waitKey();
}

// x방향의 gradient를 구하고 화면에 보여주는 함수.
void gradXFilter(Mat* mat, int width, int height) {
    Mat src;
    (*mat).copyTo(src);
    
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
            xConvolvMat[x][y] = x_gra;
            
            if (min > x_gra)
                min = x_gra;
            if (max < x_gra)
                max = x_gra;
        }
    }
    
    // gradient의 최소와 최대값
    // -237 234
    cout << min << " " << max << endl;
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // 시각화를 위해 음수를 양수로 만들어주기 위해
            // 그만큼 더해준 뒤 폭의 scaling을 위해
            // 절반으로 나눠준다.
            int x_gra = (xConvolvMat[x][y] + 238) / 2;
            int in = x_gra;
        
            // Mat 구조체에 보정된 gradient 값을 저장한다.
            src.at<uchar>(x, y) = in;
        }
    }
    
    // 필터링 된 이미지를 화면에 보여준다.
    showImage("xgrad", src);
}

// y 방향의 gradient를 구하고 화면에 보여주는 함수.
void gradYFilter(Mat* mat, int width, int height) {
    Mat src;
    (*mat).copyTo(src);
    
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
            yConvolvMat[x][y] = y_gra;
            
            if (min > y_gra)
                min = y_gra;
            if (max < y_gra)
                max = y_gra;
        }
    }
    
    // -231, 244
    cout << min << " " << max << endl;
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            
            // 역시 음수를 양수로 만들고 2로 나눠
            // gradient 값을 보정한다.
            int y_gra = (yConvolvMat[x][y] + 232) / 2;
            int in = y_gra;
            
            src.at<uchar>(x, y) = in;
        }
    }
    
    // y 방향의 gradient를 화면에 보여준다.
    showImage("ygrad", src);
}

// x방향과 y방향의 gradient를 가지고 magnitude를 구한다.
void gradFilter(Mat* mat, int width, int height) {
    gradXFilter(mat, width, height);
    gradYFilter(mat, width, height);
    // 위 두 함수가 수행되면 전역 변수에 gradient가 저장된다.
    
    Mat src = (*mat).clone();
    
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            // x와 y의 gradient를 이용해 magnitude를 구한다.
            float magnitude = sqrt(xConvolvMat[x][y] * xConvolvMat[x][y] + yConvolvMat[x][y] * yConvolvMat[x][y]);
            src.at<uchar>(x, y) = magnitude;
            
            cout << magnitude << " ";
        }
        cout << endl;
    }
    
    // 최종적인 derivative filter를 적용한 이미지를 화면에 출력한다.
    showImage("grad", src);
}
