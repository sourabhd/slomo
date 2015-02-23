#include "SloMo.hpp"
#include "delaunay2.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
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

/**
* - Select points
* - Delaunay triangulate
*/

void SloMo::triangulate(const int rows, const int cols, const int blockSize,
                        vector<vector<Point2f> > &tri)
{
    vector<Point2f> pts;

    for (int x = 0 ; x < rows ; x += blockSize) {
        Point2f pt(x, 0);
        pts.push_back(pt);
        Point2f pt2(x, cols-1);
        pts.push_back(pt2);
    }


    for (int y = blockSize ; y < cols-1 ; y += blockSize) {
        Point2f pt(0, y);
        pts.push_back(pt);
        Point2f pt2(rows-1, y);
        pts.push_back(pt2);
    }


    for (int x = blockSize ; x < rows ; x += blockSize) {
        for (int y = blockSize ; y < cols; y += blockSize) {
            Point2f pt(x, y);
            pts.push_back(pt);
        }
//        if (((cols - 1) % blockSize) != 0) {
//            pts.push_back(Point2f(cols - 1, y));
//        }
    }

//    if (((rows - 1) % blockSize) != 0) {
//        for (int x = 0 ; x < cols ; x += blockSize) {
//            pts.push_back(Point2f(x, rows-1));
//        }
//    }

    cerr << "triangulate: " << rows << " " << cols << endl;
    delaunay(pts, rows, cols, tri);
}

/**
 * finds affine transformation for inverse warping
 */

Mat SloMo::inverseAffine(vector<Point2f> &src, vector<Point2f> &dst)
{
    Mat Ainv;
    Mat A = getAffineTransform(src, dst);
    invertAffineTransform(A, Ainv);
    return Ainv;
}

void SloMo::inverseWarp(const Mat &flow, vector<vector<Point2f> > &tri)
{
    const int M = int(flow.rows);
    const int N = int(flow.cols);
    const int T = int(tri.size());
    cerr << M << " " << N << " " << T << endl << flush;


    for (int t = 0 ; t < T ; t++) { // for each triangle
        cerr << t << " ... " << endl<< flush;
        //if (t < M+2) continue;
        cerr << tri[t] << endl << flush;
        // Find inverse mapping
        vector<Point2f> srcA, dstA;
        srcA.clear(); dstA.clear();
        for (int k = 0 ; k < int(tri[t].size()) ; k++) {
            dstA.push_back(tri[t][k]);
            Point2f fxy = flow.at<Point2f>(cvRound(tri[t][k].x), cvRound(tri[t][k].y));
            Point2f src = Point2f(cvRound(tri[t][k].x+fxy.x), cvRound(tri[t][k].y+fxy.y));
            srcA.push_back(src);
        }
        Mat InvAff = inverseAffine(srcA, dstA);


        // find the bounding box
        float minX = M, minY = N, maxX = 0, maxY = 0;
        for (int k = 0 ; k < int(tri[t].size()) ; k++) {
            if (tri[t][k].x < minX) {
                minX = tri[t][k].x;
            }
            if (tri[t][k].x > maxX) {
                maxX = tri[t][k].x;
            }
            if (tri[t][k].y < minY) {
                minY = tri[t][k].y;
            }
            if (tri[t][k].y > maxY) {
                maxY = tri[t][k].y;
            }
        }

        cerr << minX << " " << maxX << " " << minY << " " << maxY << endl;

# if 0

        // Inverse map each point in the bounding box if it lies within triangle t
        for (int i = int(minX) ; i <= maxX ; i++) {
            for (int j = int(minY) ; j <= maxY ; j++) {
                Point2f pt(i,j);
                if (pointPolygonTest(tri[t], pt, false) != -1) {
                    Mat P = Mat::ones(3, 1, CV_64FC1);
                    P.at<double>(0,0) = pt.x;
                    P.at<double>(0,1) = pt.y;
                    P.at<double>(0,2) = 1.0;
                    Mat prevLoc = InvAff * P;
                    cout << prevLoc << endl << flush;
                };
            }
        }
#endif

    }
}

void SloMo::slowdown(string const& inFilename, string const outFilename)
{

#if 0
    vector<Point2f> tri;
    tri.push_back(Point2f(0,0));
    tri.push_back(Point2f(1,0));
    tri.push_back(Point2f(0.5, 0.5*sqrt(3)));
    Point2f pt(0.5,0.5);
    cout << pointPolygonTest(tri, pt, false) << endl;

    return;
#endif

    int inNumFrames = 0, outNumFrames = 0;
    const int blockSize = 16;
    bool firstFrame = true, firstFrame2 = true;
    vector<vector<Point2f> > tri;

    VideoCapture cap(inFilename);

    if( !cap.isOpened() )
        return;
    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        cout << "Frame # " << inNumFrames << endl << flush;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if (firstFrame) {

            triangulate(frame.rows, frame.cols, blockSize, tri);
            firstFrame = false;
        }

//        for (int kk = 0 ; kk < 10 ; kk++) {
//            //cerr << facets[kk] << endl;
//            cerr << centers[kk] << endl;
//        }
//        return;

        if (!prevgray.empty()) {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            //if(firstFrame2) {
             //   firstFrame2 = false;
            //} else {
            inverseWarp(flow,tri);
            //}
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
        }

        //if (waitKey(30) >= 0)
        //    break;

        std::swap(prevgray, gray);
        inNumFrames++;
    }

    cout << "Input Video : number of frames : " << inNumFrames << endl;
    cout << "Output Video: number of frames : " << outNumFrames << endl;

}