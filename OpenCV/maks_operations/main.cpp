// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月16日 星期一 09时32分45秒
/*
    遍历图片元素，并进行滤波/对比度增强处理
*/
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void Sharpen(const Mat &myImage,Mat &Result){
    CV_Assert(myImage.depth() == CV_8U);
    const int nChannels = myImage.channels();
    Result.create(myImage.size(),myImage.type());

    for(int j = 1; j < myImage.rows -1;++j){
        const uchar *previous = myImage.ptr<uchar>(j-1);
        const uchar *current = myImage.ptr(j);
        const uchar *next = myImage.ptr(j+1);
        
        uchar *output = Result.ptr<uchar>(j);
        for(int i = nChannels;i < nChannels*(myImage.cols -1); ++i){
            *output++ = saturate_cast<uchar>(5*current[i] - 
                current[i - nChannels] - current[i+nChannels] - previous[i] - next[i]);

        }
    }
    //边界处理
    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows-1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols -1).setTo(Scalar(0));

}
int main(int argc,char **argv){
    const char *filename = argc >= 2 ? argv[1]:"le.jpg";
    Mat src,dst0,dst1;
    
    if(argc >=3 && !strcmp("G",argv[2])){
        src = imread(filename,IMREAD_GRAYSCALE);
    }
    else
        src = imread(filename,IMREAD_COLOR);

    namedWindow("Input",WINDOW_AUTOSIZE);
    namedWindow("Output",WINDOW_AUTOSIZE);
    imshow("Input",src);

    double t = (double)getTickCount();
    Sharpen(src,dst0);
    t=((double)getTickCount()-t)/getTickFrequency();
    cout<<"Hand written funciton time is :"<<t<<"s"<<endl;
    
    imshow("Output",dst0);
    waitKey(0);

    Mat kernel = (Mat_<char>(3,3)<<0,-1,0,
                                    -1,5,-1,
                                    0,-1,0);
    t = getTickCount();
    filter2D(src,dst1,src.depth(),kernel);
    t=((double)getTickCount()-t)/getTickFrequency();
    cout<<"Build-in filter2D funciton time is :"<<t<<"s"<<endl;
    imshow("Output",dst1);
    waitKey(0);
    return 0;
}
