//
//  main.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/09.
//

#include "gradient.hpp"

using namespace std;
using namespace cv;

extern int** xConvolvMat;
extern int** yConvolvMat;

int main(int argc, const char * argv[]) {
    // 이미지 파일을 회색으로 읽어들인다. 해당 정보는 OpenCV의 Mat 구조체에 저장된다.
    Mat src = imread("/Users/bambookim/Desktop/CVProject/CVProject/SgQKf.jpg", IMREAD_GRAYSCALE);

    // 이미지의 가로, 세로, 채널 정보를 얻는다.
    int width = src.size().width;
    int height = src.size().height;
    int channel = src.channels();
    
    cout << width << " " << height << endl;
    
    // 전역 변수 2차원 배열 xConvolvMat와 yConvolvMat를 동적 할당한다.
    xConvolvMat = new int*[width];
    yConvolvMat = new int*[width];
    for (int i = 0; i < width; i++) {
        xConvolvMat[i] = new int[height];
        yConvolvMat[i] = new int[height];
    }
    
    // 정의한 gradFilter함수를 실행시킨다.
    gradFilter(&src, width, height);
    
    // 동적 할당을 해제한다.
    for (int i = 0; i < width; i++) {
        delete[] xConvolvMat[i];
        delete[] yConvolvMat[i];
    }
    delete[] xConvolvMat;
    delete[] yConvolvMat;
    
    return 0;
}
