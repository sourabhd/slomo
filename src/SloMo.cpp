#include "SloMo.hpp"
#include "delaunay2.hpp"
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
#include "common.hpp"

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
                        vector<vector<Point2f> > &tri,
                        unordered_map<Point2i, int, std::Point2iHash > &pointToTri,
                        Mat &edges)
{
    vector<Point2f> pts;

    // Points on an uniform grid

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

    // cerr << "triangulate: " << rows << " " << cols << endl;


    // Add points along edges
    for (int i = 0 ; i < edges.rows ; i++) {
        for (int j = 0 ; j < edges.cols ; j++) {
            int v = static_cast<int>(edges.at<unsigned char>(i,j));
            if (v != 0) {
                pts.push_back(Point2f(i,j));
            }
        }
    }

    cerr << pts.size() << " points" << endl << flush;

    delaunay(pts, rows, cols, tri);

    const int M = int(rows);
    const int N = int(cols);
    const int T = tri.size();
    for (int t = 0 ; t < T ; t++) { // for each triangle

        // cerr << t << " ... " << endl<< flush;
        // cerr << tri[t] << endl << flush;

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

        // hash map pointToTri returns the triangle index given point (i,j)
        for (int i = int(minX) ; i <= maxX ; i++) {
            for (int j = int(minY); j <= maxY; j++) {
                Point2i pt(i,j);
                if (pointPolygonTest(tri[t], pt, false) != -1) {
                    pointToTri[pt] = t;
                }
            }
        }
    }
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


//void SloMo::inverseWarp(const Mat &flow, const vector<vector<Point2f> > &tri,
//        const Mat &prevFrame, Mat &warpFrame,
//        unordered_map<Point2i, int, std::Point2iHash > &pointToTri)
//{
//    const int M = int(flow.rows);
//    const int N = int(flow.cols);
//    const int T = int(tri.size());
//    // cerr << M << " " << N << " " << T << endl << flush;
//
//    Mat warpFrameT;
//    vector<Mat> invAffTrans;
//
//    for (int t = 0 ; t < T ; t++) {
//
//        // Find inverse mapping
//        vector<Point2f> srcA, dstA;
//        srcA.clear();
//        dstA.clear();
//        for (int k = 0; k < int(tri[t].size()); k++) {
//            dstA.push_back(tri[t][k]);
//            Point2f fxy = flow.at<Point2f>(cvRound(tri[t][k].x), cvRound(tri[t][k].y));
//            Point2f src = Point2f(cvRound(tri[t][k].x + fxy.x), cvRound(tri[t][k].y + fxy.y));
//            srcA.push_back(src);
//        }
//        Mat invAff = inverseAffine(srcA, dstA);
//        invAffTrans.push_back(invAff);
//    }
//
//    Mat wmap, wempty;
//    wmap = Mat::zeros(prevFrame.size(), CV_32FC2);
//
//    // cerr << minX << " " << maxX << " " << minY << " " << maxY << endl;
//
//    // vector<float> prevLocX, prevLocY;
//    vector<Point2i> pts;
//    pts.clear();
//    // Inverse map each point in the bounding box if it lies within triangle t
//    for (int i = 0 ; i < M ; i++) {
//        for (int j = 0 ; j < N ; j++) {
//            const Point2i &pt = Point2i(i,j);
//            if (pointToTri.find(pt) != pointToTri.end()) {
//                int tnum = pointToTri[pt];
//            //if (pointPolygonTest(tri[t], pt, false) != -1) {
//                Mat P = Mat::ones(3, 1, CV_64F);
//                P.at<double>(0, 0) = double(pt.x);
//                P.at<double>(1, 0) = double(pt.y);
//                P.at<double>(2, 0) = 1.0;
//                Mat pLoc = invAffTrans[tnum] * P;
////              cerr << pt.x << " " << pt.y << endl;
////              cerr << InvAff << endl << flush;
////              cerr << pLoc << endl << flush;
////              cerr << endl << endl << flush;
//                wmap.at<Point2f>(i,j) = \
//                    Point2f(float(pLoc.at<double>(0, 0)), float(pLoc.at<double>(1, 0)));
//                pts.push_back(pt);
//            }
//        }
//    }
//
//    remap(prevFrame.t(), warpFrameT, wmap.t(), wempty, CV_INTER_LINEAR);
//
//    warpFrame = warpFrameT.t();
//}



void SloMo::inverseWarpSingle(const Mat &flow, const double alpha,
                              const vector<vector<Point2f> > &tri,
                              const Mat &prevFrame, Mat &warpFrame,
                              unordered_map<Point2i, int, std::Point2iHash > &pointToTri)
{
    const int M = int(flow.rows);
    const int N = int(flow.cols);
    const int T = int(tri.size());
    // cerr << M << " " << N << " " << T << endl << flush;

    Mat warpFrameT;
    vector<Mat> invAffTrans;

    for (int t = 0 ; t < T ; t++) {

        // Find inverse mapping
        vector<Point2f> srcA, dstA;
        srcA.clear();
        dstA.clear();
        for (int k = 0; k < int(tri[t].size()); k++) {
            dstA.push_back(tri[t][k]);
            Point2f fxy = flow.at<Point2f>(cvRound(tri[t][k].x), cvRound(tri[t][k].y));
            Point2f src = Point2f(cvRound(tri[t][k].x + alpha*fxy.x), cvRound(tri[t][k].y + alpha*fxy.y));
            srcA.push_back(src);
        }
        Mat invAff = inverseAffine(srcA, dstA);
        invAffTrans.push_back(invAff);
    }

    Mat wmap, wempty;
    wmap = Mat::zeros(prevFrame.size(), CV_32FC2);

    // cerr << minX << " " << maxX << " " << minY << " " << maxY << endl;

    vector<Point2i> pts;
    pts.clear();
    // Inverse map each point in the bounding box if it lies within triangle t
    for (int i = 0 ; i < M ; i++) {
        for (int j = 0 ; j < N ; j++) {
            const Point2i &pt = Point2i(i,j);
            if (pointToTri.find(pt) != pointToTri.end()) {
                int tnum = pointToTri[pt];
                //if (pointPolygonTest(tri[t], pt, false) != -1) {
                Mat P = Mat::ones(3, 1, CV_64F);
                P.at<double>(0, 0) = double(pt.x);
                P.at<double>(1, 0) = double(pt.y);
                P.at<double>(2, 0) = 1.0;
                Mat pLoc = invAffTrans[tnum] * P;
//              cerr << pt.x << " " << pt.y << endl;
//              cerr << InvAff << endl << flush;
//              cerr << pLoc << endl << flush;
//              cerr << endl << endl << flush;
                wmap.at<Point2f>(i,j) = \
                    Point2f(float(pLoc.at<double>(0, 0)), float(pLoc.at<double>(1, 0)));
                pts.push_back(pt);
            }
        }
    }

    remap(prevFrame.t(), warpFrameT, wmap.t(), wempty, CV_INTER_LINEAR);
    warpFrame = warpFrameT.t();

}


void SloMo::inverseWarpAll(const Mat &flow, const int factor,
                           const vector<vector<Point2f> > &tri,
                           const Mat &prevFrame, Mat &warpFrame,
                           unordered_map<Point2i, int, std::Point2iHash > &pointToTri)
{
    const float alphaIncr = 1.0 / float(factor);

    for (int i = 1 ; i <= factor ; i++) {
        float alpha = i * alphaIncr;
        inverseWarpSingle(flow, alpha, tri, prevFrame, warpFrame, pointToTri);
    }
}


void SloMo::dumpVideoProp(VideoCapture &cap)
{
    int ex = static_cast<int>(cap.get(CAP_PROP_FOURCC));
    char EXT[] = {ex & 0XFF , (ex & 0XFF00) >> 8,(ex & 0XFF0000) >> 16,(ex & 0XFF000000) >> 24, 0};
    cout << "CV_CAP_PROP_POS_MSEC      : " << setw(25) << cap.get(CV_CAP_PROP_POS_MSEC     )  << "  : Current position of the video file in milliseconds or video capture timestamp."   << endl;
    cout << "CV_CAP_PROP_POS_FRAMES    : " << setw(25) << cap.get(CV_CAP_PROP_POS_FRAMES   )  << "  : 0-based index of the frame to be decoded/captured next."                          << endl;
    cout << "CV_CAP_PROP_POS_AVI_RATIO : " << setw(25) << cap.get(CV_CAP_PROP_POS_AVI_RATIO)  << "  : Relative position of the video file: 0 - start of the film, 1 - end of the film." << endl;
    cout << "CV_CAP_PROP_FRAME_WIDTH   : " << setw(25) << cap.get(CV_CAP_PROP_FRAME_WIDTH  )  << "  : Width of the frames in the video stream."                                         << endl;
    cout << "CV_CAP_PROP_FRAME_HEIGHT  : " << setw(25) << cap.get(CV_CAP_PROP_FRAME_HEIGHT )  << "  : Height of the frames in the video stream."                                        << endl;
    cout << "CV_CAP_PROP_FPS           : " << setw(25) << cap.get(CV_CAP_PROP_FPS          )  << "  : Frame rate."                                                                      << endl;
    cout << "CV_CAP_PROP_FOURCC        : " << setw(25) << EXT                                 << "  : 4-character code of codec."                                                       << endl;
    cout << "CV_CAP_PROP_FRAME_COUNT   : " << setw(25) << cap.get(CV_CAP_PROP_FRAME_COUNT  )  << "  : Number of frames in the video file."                                              << endl;
    cout << "CV_CAP_PROP_FORMAT        : " << setw(25) << cap.get(CV_CAP_PROP_FORMAT       )  << "  : Format of the Mat objects returned by retrieve() ."                               << endl;
    cout << "CV_CAP_PROP_MODE          : " << setw(25) << cap.get(CV_CAP_PROP_MODE         )  << "  : Backend-specific value indicating the current capture mode."                      << endl;
    cout << "CV_CAP_PROP_CONVERT_RGB   : " << setw(25) << cap.get(CV_CAP_PROP_CONVERT_RGB  )  << "  : Boolean flags indicating whether images should be converted to RGB."              << endl;
    cout << flush;
}

void SloMo::slowdown(string const& inFilename, string const outFilename, const int factor) {




    int inNumFrames = 0, outNumFrames = 0;
    bool firstFrame = true, firstFrame2 = true;
    // vector<vector<Point2f> > tri;
    // unordered_map<Point2i, int, std::Point2iHash> pointToTri;

    VideoCapture cap(inFilename);

    if (!cap.isOpened()) {
        cerr << "Could not open input file " << inFilename << endl << flush;
        return;
    }

    dumpVideoProp(cap);

    //VideoWriter::fourcc('X', '2', '6', '4'),
    VideoWriter vw(outFilename,
                   static_cast<int>(cap.get(CV_CAP_PROP_FOURCC)),
                   cap.get(CV_CAP_PROP_FPS),
                   Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT))
    );

    if (!vw.isOpened()) {
        cerr << "Could not open output file " << outFilename << endl << flush;
        return;
    }

    Mat flow, cflow, frame, prevframe, prevframeN, wframe, wframeN;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    const float incrAlpha = float(1) / float(factor);

    for(;;) {

        TIME_START(3)
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        cout << "Frame # " << inNumFrames << endl << flush;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        Mat edges;
        Scalar meanVal = mean(gray);
        double cannyThreshold1 = 0.33 * meanVal.val[0];
        double cannyThreshold2 = 1.33 * meanVal.val[0];
        Canny(gray, edges, cannyThreshold1, cannyThreshold2, 3, true);
        // Mat edgesD;
        // edges.convertTo(edgesD, CV_8UC1, 255, 0);
        // imshow("flow", edgesD);
        // cvWaitKey(0);

        Mat frameN;
        frame.convertTo(frameN, CV_32FC3, 1.0/255.0, 0);
        wframeN = Mat::zeros(frame.rows, frame.cols, CV_32FC3);

        // if (firstFrame) {

            // Triangulate

            TIME_START(2)
            vector<vector<Point2f> > tri;
            unordered_map<Point2i, int, std::Point2iHash> pointToTri;
            triangulate(frame.rows, frame.cols, blockSize, tri, pointToTri,edges);
            firstFrame = false;
            TIME_END(2, "Traiangulation")

        if (firstFrame) {
            vw.write(frame);
        }


        if (!prevgray.empty()) {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);

            // Do the warping
            prevframe.convertTo(prevframeN, CV_32FC3, 1.0/225.0, 0);
            inverseWarpAll(flow, factor, tri, prevframeN, wframeN, pointToTri);
            wframeN.convertTo(wframe, frame.type(), 255.0, 0);



//            // Cross Dissolve from prev to warped
//
//            for (float alpha = incrAlpha ; alpha < 1.0f ; alpha += incrAlpha) {
//                //Mat iframeN = alpha * frameN + (1-alpha) * wframeN;
//                Mat iframeN = alpha * wframeN + (1-alpha) * prevframeN;
//                Mat iframe;
//                iframeN.convertTo(iframe, frame.type(), 255.0, 0);
//                vw.write(iframe);
//            }
//


            // cerr << frame.cols << " " << flow.rows << endl;
            // cerr << flow.cols << " " << flow.rows << endl;
            // cerr << wframe.cols << " " << wframe.rows << endl;
            // vw.write(frame);
            //vw.write(prevframe);

            vw.write(wframe);

            //drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            // imshow("flow", cflow);

            //imshow("flow", wframeN.t());
            //cvWaitKey(0);
        }

        //if (waitKey(30) >= 0)
        //    break;

        std::swap(prevgray, gray);
        std::swap(prevframe,frame);
        inNumFrames++;
        outNumFrames++;
        TIME_END(3, "Frame")
    }


//    for (int i = 0 ; i < factor-1 ; i++) {
//        vw.write(prevframe);
//    }


    cap.release();
    vw.release();

    cout << "Input Video : number of frames : " << inNumFrames << endl << flush;
    cout << "Output Video: number of frames : " << outNumFrames << endl << flush;

    cout << "Output video stats: " << endl << flush;

    VideoCapture cap2(outFilename);

    if (!cap2.isOpened()) {
        cerr << "Could not open output file " << outFilename << endl << flush;
        return;
    }

    dumpVideoProp(cap2);
    cap2.release();

}