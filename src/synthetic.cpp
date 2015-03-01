/**
* @author: Sourabh Daptardar <saurabh.daptardar@gmail.com>
*/



#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

void createSynVideo(const string &videofname)
{
    const int w = 1000, h = 1000;
    const int numFrames = 100;
    const int disp_x = 100, disp_y = 100;
    const int radius = 10;
    const int fps = 24;
    Mat img(w, h, CV_8UC3, Scalar::all(0    ));
    circle(img, Point(radius,radius), radius, Scalar(255,0,0), -1);

    Mat M = Mat::zeros(2, 3, CV_64FC1);
    M.at<double>(0,0) = 1;
    M.at<double>(0,1) = 0;
    M.at<double>(0,2) = disp_x;
    M.at<double>(1,0) = 0;
    M.at<double>(1,1) = 1;
    M.at<double>(1,2) = disp_y;
    VideoWriter vw(videofname, VideoWriter::fourcc('A','V','C','1'), fps, Size(w,h));
    for (int i = 0 ; i < numFrames ; i++) {
        Mat timg;
        warpAffine(img, timg, M, Size(img.cols,img.rows));
        vw.write(timg);
        std::swap(img,timg);
    }
    vw.release();

}