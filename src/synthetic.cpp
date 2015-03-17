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

void draw_line(Mat &img, const Point &start, const Point &end)
{

    const int thickness = 2;
    const int lineType = 8;
    line(img, start, end, Scalar(0,255,0), thickness, lineType);
} 

void createSynVideo(const string &videofname)
{
    const int w = 1000, h = 1000;
    const int numFrames = 100;
    const int disp_x = h/numFrames, disp_y = w/numFrames;
    const int radius = 50;
    const int fps = 24;
    Mat img(w, h, CV_8UC3, Scalar::all(0));
    circle(img, Point(radius-1,radius-1), radius, Scalar(255,0,0), -1);

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
//        for (int x = 0 ; x < h ; x += disp_x) {
//            draw_line(timg, Point(x, 0), Point(x, w-1));
//        }
//        for (int y = 0 ; y < w ; y += disp_y) {
//            draw_line(timg, Point(0,y), Point(h-1,y));
//        }

        vw.write(timg);
        std::swap(img,timg);
    }
    vw.release();

}

int main(int argc, char *argv[])
{
    const string testVideo = "data/test.avi";
    createSynVideo(testVideo);
    return 0;
}
