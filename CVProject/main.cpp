//
//  main.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/09.
//

#include <iostream>
#include "gradient.hpp"
#include "gaussian.hpp"
#include "derivOfGaussian.hpp"
#include "canny.hpp"

using namespace std;
using namespace cv;

extern double** xConvolvMat;
extern double** yConvolvMat;
extern double** magnitudes;

void initGlobalArray(int width, int height) {
    xConvolvMat = new double*[height];
    yConvolvMat = new double*[height];
    magnitudes = new double*[height];
    for (int i = 0; i < height; i++) {
        xConvolvMat[i] = new double[width];
        yConvolvMat[i] = new double[width];
        magnitudes[i] = new double[width];
    }
}

void deleteGlobalArray(int height) {
    for (int i = 0; i < height; i++) {
        delete[] xConvolvMat[i];
        delete[] yConvolvMat[i];
        delete[] magnitudes[i];
    }
    delete[] xConvolvMat;
    delete[] yConvolvMat;
    delete[] magnitudes;
}

int main(int argc, const char * argv[]) {
    // 이미지 파일을 회색으로 읽어들인다. 해당 정보는 OpenCV의 Mat 구조체에 저장된다.
    string fileName;
    //cin >> fileName;
    fileName = "IMG_5624.jpeg";
    String filePath = "/Users/bambookim/Desktop/CVProject/CVProject/" + fileName;
    Mat src = imread(filePath, IMREAD_GRAYSCALE);

    // 이미지의 가로, 세로, 채널 정보를 얻는다.
    int width = src.size().width;
    int height = src.size().height;
    int channel = src.channels();
    
    cout << "width: " << width << endl;
    cout << "height: " << height << endl;
    cout << "channel: " << channel << endl;
    
    // 전역 변수 2차원 배열 xConvolvMat와 yConvolvMat를 동적 할당한다.
    initGlobalArray(width, height);
    
    int kernelSize = 15;
    int sigma = 3;
    int high = 10;
    int mid = 7;
    int low = 5;
    
    //src = gradFilter(&src);
    //src = gaussianFilter(&src, 15, 5);
    //src = gradFilter(&src);
    
    
    
    imshow("Source", src);
    waitKey();
    
    Mat CannyImg = src.clone();
    
    CannyImg = gaussianFilter(&CannyImg, kernelSize, sigma);
    //imshow("Gaussian", CannyImg);
    //waitKey();
    
    CannyImg = gradFilter(&CannyImg);
    //imshow("Gaussian-Derivative", CannyImg);
    //waitKey();
    
    Mat CannyDouble, CannyTriple;

    CannyDouble = CannyImg.clone();
    CannyDouble = cannyEdgeDouble(&CannyDouble, high, low);
    imshow("Canny Edge Double", CannyDouble);
    waitKey();
    
    imwrite("/Users/bambookim/Desktop/CVProject2/CVProject/result/report/jongseong_canny_gray.jpg", CannyDouble);
    /*
    CannyTriple = CannyImg.clone();
    CannyTriple = cannyEdgeTriple(&CannyTriple, high, mid, low);
    imshow("Canny Edge Triple", CannyTriple);
    waitKey();
    
    cout << "sigma: " << sigma << endl;
    cout << " high: " << high << endl;
    cout << "  mid: " << mid << endl;
    cout << "  low: " << low << endl;
    
    String doubleName = "s" + to_string(sigma) + "_h" + to_string(high) + "_l" + to_string(low) + ".jpeg";
    String tripleName = "s" + to_string(sigma) + "_h" + to_string(high) + "_m" + to_string(mid) + "_l" + to_string(low) + ".jpeg";
    
    imwrite("/Users/bambookim/Desktop/CVProject/CVProject/result/" + doubleName, CannyDouble);
    imwrite("/Users/bambookim/Desktop/CVProject/CVProject/result/" + tripleName, CannyTriple);
    */
    
    cout << "Saved Images." << endl;
    
    /*
    String cmd;
    int num;
    int count = 0;
    while (true) {
        cout << "cmd >> ";
        cin >> cmd;
        
        if (cmd == "q") {
            break;
        } else if (cmd == "m" || cmd == "h" || cmd == "l") {
            cout << "num >> ";
            cin >> num;
            
            if (cmd == "h") {
                high = num;
            } else if (cmd == "m") {
                mid = num;
            } else if (cmd == "l") {
                low = num;
            }
        }
        
        CannyDouble = CannyImg.clone();
        CannyDouble = cannyEdgeDouble(&CannyDouble, high, low);
        //imshow("Canny Edge Double", CannyDouble);
        //waitKey();
               
        CannyTriple = CannyImg.clone();
        CannyTriple = cannyEdgeTriple(&CannyTriple, high, mid, low);
        //imshow("Canny Edge Triple", CannyTriple);
        //waitKey();
        
        cout << "sigma: " << sigma << endl;
        cout << " high: " << high << endl;
        cout << "  mid: " << mid << endl;
        cout << "  low: " << low << endl;
        
        String doubleName = to_string(count) + "s" + to_string(sigma) + "_h" + to_string(high) + "_l" + to_string(low) + ".jpeg";
        String tripleName = to_string(count) + "s" + to_string(sigma) + "_h" + to_string(high) + "_m" + to_string(mid) + "_l" + to_string(low) + ".jpeg";
        
        imwrite("/Users/bambookim/Desktop/CVProject/CVProject/result/" + doubleName, CannyDouble);
        imwrite("/Users/bambookim/Desktop/CVProject/CVProject/result/" + tripleName, CannyTriple);
        
        count++;
        
        cout << "Saved Images." << endl;
    }
    */

    // 동적 할당을 해제한다.
    deleteGlobalArray(height);
    
    return 0;
}
