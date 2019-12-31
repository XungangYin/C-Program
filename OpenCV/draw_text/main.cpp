// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月23日 星期一 17时03分14秒
/*\
    绘制文字
*/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static Scalar randomColor(RNG &rng){
    int icolor = (unsigned) rng;
    return Scalar(icolor&255,(icolor>>8)&255,(icolor>>16)&255);
}

int main(int argc,char **argv){

    Mat img(400,400,CV_8UC3);

    int lineType = 8;
    RNG rng;

    for(int i = 0; i < 4;i++){
        Point org;
        org.x = rng.uniform(50,300);
        org.y = rng.uniform(100,200);
        putText(img,"中文 Texting rendering",org,rng.uniform(0,8),
                rng.uniform(0,100)*0.05+0.1,randomColor(rng),
                rng.uniform(1,10),lineType);
    }
    imshow("TEXT",img);
    waitKey(0);

    return 0;    
}
