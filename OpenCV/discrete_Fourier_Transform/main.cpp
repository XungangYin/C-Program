// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月18日 星期三 19时44分15秒
/**
    离散傅里叶变化(其结果是复数).可将图片的空间域转换为频谱域
*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

static void help(char **argv){
    cout<<endl
        <<"This program demonstrated the use of discrete Fourire Transform"<<endl;
}
int main(int argc,char **argv){
    help(argv);
    const char *filename = argc >=2 ? argv[1]:"1.png";
    Mat I = imread(filename,0);
    //为执行傅里叶变换做准备
    Mat padded;
    //获取最佳参数(2的n次幂)
    int m = getOptimalDFTSize(I.rows);
    int n = getOptimalDFTSize(I.cols);
    //填充边界
    copyMakeBorder(I,padded,0,m-I.rows,0,n-I.cols,BORDER_CONSTANT,Scalar::all(0));
    
    imshow("padded img",padded);
    imshow("原始图像",I);
    
    Mat planes[] = {Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F)};
    Mat complexI;
    merge(planes,2,complexI);//合并.函数作用是:从多个单通道创建一个多通道矩阵
    //    imshow("planes0 ",planes[0]);
    dft(complexI,complexI);//执行傅里叶变换

    //计算幅值compute the magnitude
    split(complexI,planes); //分解矩阵.planes[0]=实部,planes[1]是虚部
    magnitude(planes[0],planes[1],planes[0]); //planes[0] = magnitude
    Mat magI = planes[0];   
    
    magI +=Scalar::all(1);
    log(magI,magI);
    
    //copy the spectrum,if it has an odd number of rows or cols;
    magI = magI(Rect(0,0,magI.cols &-2,magI.rows &-2)); //为什么用 按位与 运算
    
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI,Rect(0,0,cx,cy));
    Mat q1(magI,Rect(cx,0,cx,cy));
    Mat q2(magI,Rect(0,cy,cx,cy));
    Mat q3(magI,Rect(cx,cy,cx,cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI,magI,0,1,NORM_MINMAX);
    imshow("Input image:",I);
    imshow("magI ",magI);
    waitKey();
    
    return EXIT_SUCCESS;    
}
