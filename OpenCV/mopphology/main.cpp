// File Name: main.cpp
// Author: YXG
// Created Time: 2019年12月26日 星期四 15时01分20秒
/*
    对图像进行侵蚀(erode)和扩张(dilate)的组合操作
*/

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat src,dst;
int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const char *window_name = "Morphology Transform Demo";

void Morphology_Operations(int ,void *){
    //Morph X:2 3 4 5 6;
    int operation = morph_operator + 2;
    Mat element = getStructuringElement(morph_elem,Size(2*morph_size +1,
                                        2*morph_size+1),
                                        Point(morph_size,morph_size));
    morphologyEx(src,dst,operation,element);
    imshow(window_name,dst);    
    
}
int main(int argc,char **argv){
    src = imread(argv[1],IMREAD_COLOR);
    if(src.empty()){
        cout<<"Could not find the file"<<endl;
        return EXIT_FAILURE;
    }
    namedWindow(window_name,WINDOW_AUTOSIZE);
    createTrackbar("Operator:\n 0:Opening -1:Closing \n 2:Gradient -3 :Top Hat \n 4:Black Hat",
                    window_name,
                    &morph_operator,max_operator,Morphology_Operations);

    createTrackbar("Element:\n 0:Rect -1:Cross - 2:Ellipse",window_name,
                    &morph_elem,max_elem,
                    Morphology_Operations);
    
    createTrackbar("Kernel size:\n 2n+1",window_name,
                    &morph_size,max_kernel_size,
                    Morphology_Operations);

    Morphology_Operations(0,0);
    waitKey(0);

    return 0;
}
