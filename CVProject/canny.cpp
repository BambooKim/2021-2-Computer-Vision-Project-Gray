//
//  canny.cpp
//  CVProject
//
//  Created by 김범구 on 2021/12/11.
//

#include "canny.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>
#include <utility>

#define threshold_high 20
#define threshold_mid 7
#define threshold_low 2.5

using namespace std;
using namespace cv;

extern double** xConvolvMat;
extern double** yConvolvMat;
extern double** magnitudes;

double** orientation;

Mat cannyEdgeDouble(Mat* mat) {
    Mat canny = (*mat).clone();
    canny = nonMaxSuppress(&canny);
    Mat thin = canny.clone();
    
    stack<pair<int, int>> Stack;
    
    int width = canny.size().width;
    int height = canny.size().height;
    
    int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    
    bool** check = new bool*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new bool[width];
    }
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (thin.at<uchar>(x, y) >= threshold_high) {
                canny.at<uchar>(x, y) = 255;
                check[y][x] = true;
                Stack.push(make_pair(x, y));
            } else {
                canny.at<uchar>(x, y) = 0;
            }
        }
    }
    
    while (!Stack.empty()) {
        int x = Stack.top().first;
        int y = Stack.top().second;
        Stack.pop();
        check[y][x] = true;
        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (0 <= nx && nx < width && 0 <= ny && ny < height) {
                if (!check[ny][nx]) {
                    double nMag = magnitudes[ny][nx];
                    if (threshold_low <= nMag && nMag <= threshold_high) {
                        //check[ny][nx] = true;
                        Stack.push(make_pair(nx, ny));
                        canny.at<uchar>(nx, ny) = 255;
                    }
                }
            }
            
        }
    }
    
    for (int i = 0; i < height; i++) {
        delete[] check[i];
    }
    delete[] check;
    
    return canny;
}

Mat cannyEdgeTriple(Mat* mat) {
    Mat canny = (*mat).clone();
    canny = nonMaxSuppress(&canny);
    Mat thin = canny.clone();

    stack<pair<int, int>> Stack;
    stack<pair<int, int>> _Stack;
    
    int width = canny.size().width;
    int height = canny.size().height;
    
    bool** check = new bool*[height];
    bool** _check = new bool*[height];
    for (int i = 0; i < height; i++) {
        check[i] = new bool[width];
        _check[i] = new bool[width];
    }
    
    int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (thin.at<uchar>(x, y) >= threshold_high) {
                canny.at<uchar>(x, y) = 255;
                check[y][x] = true;
                _check[y][x] = true;
                Stack.push(make_pair(x, y));
            } else {
                canny.at<uchar>(x, y) = 0;
            }
        }
    }
    
    while (!Stack.empty()) {
        int x = Stack.top().first;
        int y = Stack.top().second;
        Stack.pop();
        check[y][x] = true;
        _check[y][x] = true;
        
        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (0 <= nx && nx < width && 0 <= ny && ny < height) {
                if (!check[ny][nx]) {
                    double nMag = magnitudes[ny][nx];
                    if (threshold_mid <= nMag && nMag <= threshold_high) {
                        //check[ny][nx] = true;
                        //_check[ny][nx] = true;
                        Stack.push(make_pair(nx, ny));
                        _Stack.push(make_pair(nx, ny));
                        canny.at<uchar>(nx, ny) = 255;
                    }
                }
            }
        }
    }
    
    while (!_Stack.empty()) {
        int x = _Stack.top().first;
        int y = _Stack.top().second;
        _Stack.pop();
        _check[y][x] = true;
        
        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (0 <= nx && nx < width && 0 <= ny && ny < height) {
                if (!_check[ny][nx]) {
                    double nMag = magnitudes[ny][nx];
                    if (threshold_low <= nMag && nMag <= threshold_mid) {
                        // _check[ny][nx] = true;
                        _Stack.push(make_pair(nx, ny));
                        canny.at<uchar>(nx, ny) = 255;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < height; i++) {
        delete[] check[i];
        delete[] _check[i];
    }
    delete[] check;
    delete[] _check;
    
    return canny;
}

Mat nonMaxSuppress(Mat* mat) {
    Mat src = *mat;
    Mat thin = src.clone();
    
    int width = src.size().width;
    int height = src.size().height;
    
    //double orientation[width][height]
    orientation = new double*[height];
    for (int i = 0; i < height; i++) {
        orientation[i] = new double[width];
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            orientation[y][x] = xConvolvMat[y][x] / yConvolvMat[y][x];
        }
    }

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            double orient = orientation[y][x];
            double mag = magnitudes[y][x];
            double alpha, beta;
            
            if (orient == 0.0) {
                // 0도 / 180도
                alpha = magnitudes[y][x - 1];
                beta = magnitudes[y][x + 1];
            } else if (!isfinite(orient)) {
                // 90도 / 270도
                alpha = magnitudes[y + 1][x];
                beta = magnitudes[y - 1][x];
            } else if (orient == 1.0) {
                // 45도 / 225도
                alpha = magnitudes[y - 1][x - 1];
                beta = magnitudes[y + 1][x + 1];
            } else if (orient == -1.0) {
                // 135도 / 315도
                alpha = magnitudes[y - 1][x + 1];
                beta = magnitudes[y + 1][x - 1];
            } else {
                // 그 외 interpolation
                if (0 < orient && orient < 1) {
                    // 0~45
                    alpha = (magnitudes[y + 1][x + 1] + magnitudes[y][x + 1]) / 2.0;
                    beta = (magnitudes[y - 1][x - 1] + magnitudes[y][x - 1]) / 2.0;
                    
                } else if (-1 < orient && orient < 0) {
                    // 135~180
                    alpha = (magnitudes[y + 1][x - 1] + magnitudes[y][x - 1]) / 2.0;
                    beta = (magnitudes[y - 1][x + 1] + magnitudes[y][x + 1]) / 2.0;
                    
                } else if (1 < orient) {
                    // 45 ~ 90
                    alpha = (magnitudes[y + 1][x] + magnitudes[y + 1][x + 1]) / 2.0;
                    beta = (magnitudes[y - 1][x] + magnitudes[y - 1][x - 1]) / 2.0;
                    
                } else {//if (-1 > orient) {
                    // 90 ~ 135
                    alpha = (magnitudes[y + 1][x + 1] + magnitudes[y][x + 1]) / 2.0;
                    beta = (magnitudes[y - 1][x - 1] + magnitudes[y][x - 1]) / 2.0;
                    
                }
            }
            
            if (mag >= alpha && mag >= beta) {
                thin.at<uchar>(x, y) = mag;
            } else {
                thin.at<uchar>(x, y) = 0;
            }
        }
    }

    return thin;
}
