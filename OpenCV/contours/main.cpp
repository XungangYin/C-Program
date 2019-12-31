// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月30日 星期一 10时31分16秒
/*
    寻找图像中实体轮廓
*/

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat src_gray;
int thresh = 100;
RNG rng(12345);
void thresh_callback(int ,void *){
    Mat canny_output;
    Canny(src_gray,canny_output,thresh,thresh*2);
    
    vector< vector<Point> >contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE);

    Mat drawing = Mat::zeros(canny_output.size(),CV_8UC3);
    for(size_t i = 0; i < contours.size();i++){
        Scalar color = Scalar(rng.uniform(0,256),rng.uniform(0,256),rng.uniform(0,256));
        drawContours(drawing,contours,(int)i,color,2,LINE_8,hierarchy,0);    
    }
    imshow("Contours",drawing);
        
}

int main(int argc,char **argv){
   Mat src = imread(argv[1],IMREAD_COLOR);
   cvtColor(src,src_gray,COLOR_BGR2GRAY);
   blur(src_gray,src_gray,Size(3,3));
   
   const char *source_window = "Source";
   namedWindow(source_window);
   imshow(source_window,src);
   
   const int max_thresh = 255;
   createTrackbar("Canny thresh",source_window,&thresh,max_thresh,thresh_callback);
   thresh_callback(0,0);
   waitKey(0);
   return 0;
      
}
