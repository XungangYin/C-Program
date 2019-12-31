// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月16日 星期一 18时36分44秒

/*
    两幅图片进行线性alpha融混
    addWeighted()函数:需要两幅图片像素和通道数一样
*/

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using std::cin;
using std::endl;
using std::cout;

int main(int argc ,char **argv){
    double alpha = 0.5;
    double beta;
    double input;

    Mat src1,src2,dst;

    cout<<"Enter alpha [0.0-1.0]:";
    cin>>input;
    if(input >=0 && input <=1){
        alpha = input;    
    }

    src1 = imread(argv[1],IMREAD_COLOR);
    src2 = imread(argv[2],IMREAD_COLOR);

    beta = (1.0-alpha);
    
    addWeighted(src1,alpha,src2,beta,0.0,dst);
    imshow("Liner blend",dst);

    waitKey(0);
    
    cout<<"进行对比度增强处理"<<endl;
    Mat new_img = Mat::zeros(src1.size(),src1.type());
    double gain;
    double bias;
    cout<<"Enter the gain[1.0-5.0] value:";cin>>gain;
    cout<<"Enter the bias[0-100] value:";cin>>bias;
    for(int y= 0;y < src1.rows;++y){
        for(int x =0; x < src1.cols;++x){
            for(int c = 0; c < src1.channels();++c){
                new_img.at<Vec3b>(y,x)[c] = 
                    saturate_cast<uchar>(gain * src1.at<Vec3b>(y,x)[c] + bias);
            }
        }
    }
    
    imshow("New img",new_img);
    waitKey();
    return 0;
}
