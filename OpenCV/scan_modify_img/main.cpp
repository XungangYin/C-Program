// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月13日 星期五 09时23分57秒
/*
    1、多种方法遍历图像像素
    2、统计每种方法的执行时间
    3、修改像素
*/


#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>

#include <sstream>
#include <iostream>

using namespace std;
using namespace cv;

//方式一 C风格
Mat &scanImageAndReduceC(Mat &I,const uchar *const table){
    CV_Assert(I.depth() == CV_8U);
    
    int channels = I.channels();
    int rows = I.rows;
    int cols = I.cols * rows;
    if(I.isContinuous()){ //判断是否在内存中是连续存贮
        cols *= rows;
        rows = 1;    
    }

    int i,j;
    uchar *p;
    for(i = 0;i < rows;++i){
        p = I.ptr<uchar>(i);    
        for( j = 0; j < cols; ++j){
            p[j] = table[p[j]];
         }
    }
    
    return I;
}

//方式二:迭代器(更安全的方式)
Mat &scanImageAndReduceIterator(Mat &I,const uchar * const table){
    CV_Assert(I.depth() == CV_8U);
    const int channels = I.channels();
    switch(channels){
        case 1:
        {
            MatIterator_<uchar> it,end;
            for(it = I.begin<uchar>(),end = I.end<uchar>();it !=end;++it){
                *it = table[*it];    
            }
            break; 
        }
        case 3:
        {
            MatIterator_<Vec3b> it,end;
            for(it = I.begin<Vec3b>(),end = I.end<Vec3b>();it != end;++it){
                (*it)[0] = table[(*it)[0]];
                (*it)[1] = table[(*it)[1]];
                (*it)[2] = table[(*it)[2]];
            }
        
            break;
        }
    }
    return I;
}

//方式三;LUT函数
Mat &LUTfunction(Mat &I,const uchar * const  table){
    Mat lookUpTable(1,256,CV_8U);
    uchar *p = lookUpTable.ptr();
    for(int i=0;i < 256;++i){
        p[i] = table[i];    
    }
    Mat J;
    LUT(I,lookUpTable,J);
    return J;
}

int main(int argc,char **argv){

    Mat I,J;
    I = imread(argv[1],IMREAD_COLOR);
    if(I.empty()){
        cout<<"The image "<<argv[1]<<"could not be loaded."<<endl;
        return -1;
    }
    

    imshow("show",I);
    //命令行输入比例因子，并转化为整数
    int divideWith = 0;
    stringstream s;
    s << argv[2];
    s >> divideWith;
    if(!s || !divideWith){
        cout<<"Invalid number entered for dividing"<<endl;
        return -1;    
    }

    //归一化处理
    uchar table[256];
    for(int i = 0; i < 256; ++i ){
        table[i] = (uchar) (divideWith *(i/divideWith));            
    }

    //取100次的平均
    const int times = 100;
    double t;
    t = (double)getTickCount();
    for(int i = 0 ;i < times; ++i){
        Mat clone = I.clone();
        J = scanImageAndReduceC(clone,table);    
    }
    t = 1000*((double)getTickCount()-t)/getTickFrequency();
    t /= times;
    cout<<"Time of C style :"<<t<<endl;


    t = (double)getTickCount();
    for(int i = 0 ;i < times; ++i){
        Mat clone = I.clone();
        J = scanImageAndReduceIterator(clone,table);    
    }
    t = 1000*((double)getTickCount()-t)/getTickFrequency();
    t /= times;
    cout<<"Time of iterator style :"<<t<<endl;


    t = (double)getTickCount();
    for(int i = 0 ;i < times; ++i){
        Mat clone = I.clone();
        J = LUTfunction(clone,table);    
    }
    t = 1000*((double)getTickCount()-t)/getTickFrequency();
    t /= times;
    cout<<"Time of C style :"<<t<<endl;


    return 0;    
}
