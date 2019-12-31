// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月12日 星期四 11时20分30秒
/*
    1、图像读取
    2、图像转化
    3、图像保存
*/
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc,char **argv){
    
    if(argc !=2){
        cout<<"请输入图片"<<endl;
        return -1;
     }
    
    Mat image;
    image = imread(argv[1],1);
    if(!image.data){
        printf("No image data \n");
        return -1;    
    }
    namedWindow("Display Image",WINDOW_AUTOSIZE);
    imshow("Display",image);
    waitKey(0);
    
    Mat gray_img;
    cvtColor(image,gray_img,COLOR_BGR2GRAY);
    imwrite("./gray_img.jpg",gray_img);
    imshow("gray img",gray_img);
    waitKey(0);

    return 0;    
}
