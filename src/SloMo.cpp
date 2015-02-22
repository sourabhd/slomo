#include "SloMo.hpp"
#include "delaunay2.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace slomo;
using namespace cv;

// Optical flow calculation code adapted from OpenCV Farenback optical flow examples at:
// https://github.com/Itseez/opencv/blob/master/samples/cpp/fback.cpp

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap,
                           int step, double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                    color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

void SloMo::triangulate(const int rows, const int cols, const int blockSize,
                        vector<vector<Point2f> > &facets, vector<Point2f> &centers)
{
    vector<Point2f> pts;
    for (int y = 0 ; y < rows ; y += blockSize) {
        for (int x = 0; x < cols; x += blockSize) {
            Point2f pt(x, y);
            pts.push_back(pt);
        }
        if (((cols - 1) % blockSize) != 0) {
            pts.push_back(Point2f(cols - 1, y));
        }
    }

    if (((rows - 1) % blockSize) != 0) {
        for (int x = 0 ; x < cols ; x += blockSize) {
            pts.push_back(Point2f(x, rows-1));
        }
    }

    cerr << "triangulate: " << rows << " " << cols << endl;
    delaunay(pts, rows, cols, facets, centers);
}

void SloMo::slowdown(string const& inFilename, string const outFilename)
{
    const int blockSize = 16;
    bool firstFrame = true;
    vector<vector<Point2f> > facets;
    vector<Point2f> centers;

    VideoCapture cap(inFilename);

    if( !cap.isOpened() )
        return;
    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;) {
        cap >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if (firstFrame) {

            triangulate(frame.rows, frame.cols, blockSize, facets, centers);
            firstFrame = false;
        }

        if (!prevgray.empty()) {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            //drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
        }

        if (waitKey(30) >= 0)
            break;

        std::swap(prevgray, gray);
    }

}