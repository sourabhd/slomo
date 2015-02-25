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
                        /* unordered_map<Point2i, int,  std::Point2iHash > &pointToTri, */
                        Mat &edges, const vector<Point2f> &corners)
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

    // Add corners
# if 1
    int numCorners = corners.size();
    for (int i = 0 ; i < numCorners ; i++) {
        //cerr << corners[i] << endl << flush;
        //cerr << "----------------------------------------" << endl<< flush;
        pts.push_back(Point2f(corners[i].y,corners[i].x));
    }
#endif

    cerr << pts.size() << " points" << endl << flush;

    delaunay(pts, rows, cols, tri);

#if 0
//    const int M = int(rows);
//    const int N = int(cols);
//    const int T = tri.size();
//    for (int t = 0 ; t < T ; t++) { // for each triangle
//
//        // cerr << t << " ... " << endl<< flush;
//        // cerr << tri[t] << endl << flush;
//
//        // find the bounding box
//        float minX = M, minY = N, maxX = 0, maxY = 0;
//        for (int k = 0 ; k < int(tri[t].size()) ; k++) {
//            if (tri[t][k].x < minX) {
//                minX = tri[t][k].x;
//            }
//            if (tri[t][k].x > maxX) {
//                maxX = tri[t][k].x;
//            }
//            if (tri[t][k].y < minY) {
//                minY = tri[t][k].y;
//            }
//            if (tri[t][k].y > maxY) {
//                maxY = tri[t][k].y;
//            }
//        }
//
//        // hash map pointToTri returns the triangle index given point (i,j)
//        for (int i = int(minX) ; i <= maxX ; i++) {
//            for (int j = int(minY); j <= maxY; j++) {
//                Point2i pt(i,j);
//                if (pointPolygonTest(tri[t], pt, false) != -1) {
//                    pointToTri[pt] = t;
//                }
//            }
//        }
//    }
//
#endif

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



void SloMo::inverseWarpSingle(const int rows,
                              const int cols,
                              const Mat &flow, const double alpha,
                              const vector<vector<Point2f> > &inTri,
                              const Mat &prevFrame, Mat &warpFrame
                              )
{

    const int M = rows;
    const int N = cols;
    const int T = int(inTri.size());
    vector<vector<Point2f> > tri;
    unordered_map<Point2i, int, std::Point2iHash > pointToTri;

    // cerr << M << " " << N << " " << T << endl << flush;

    // Interpolate triangulated points (geometry)
    // Inverse mappings



    Mat wframeN = Mat::zeros(M, N, CV_32FC3);
    Mat warpFrameT;
    vector<Mat> invAffTrans;

    for (int t = 0 ; t < T ; t++) {

        //cerr << "t = " << t << endl << flush;
        // Find inverse mapping
        vector<Point2f> srcA, dstA;
        srcA.clear();
        dstA.clear();
        for (int k = 0; k < int(inTri[t].size()); k++) {
            dstA.push_back(inTri[t][k]);
            Point2f fxy = flow.at<Point2f>(cvRound(inTri[t][k].x), cvRound(inTri[t][k].y));
            Point2f src = Point2f(cvRound(inTri[t][k].x + alpha*fxy.x), cvRound(inTri[t][k].y + alpha*fxy.y));
            srcA.push_back(src);
        }
        tri.push_back(srcA);   // Source triangulation
        Mat invAff = inverseAffine(srcA, dstA);
        invAffTrans.push_back(invAff);


        //  Find bounding box of each triangle
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

    // remap is equivalent of Matlab interp2
    remap(prevFrame.t(), warpFrameT, wmap.t(), wempty, CV_INTER_LINEAR);
    warpFrame = warpFrameT.t();

}


void SloMo::inverseWarpAll(const int rows,
                           const int cols,
                           const Mat &flow,
                           const int factor,
                           const vector<vector<Point2f> > &tri,
                           const Mat &prevFrame,
                           vector<Mat> &warpFrameVector
                           )
{
    const float alphaIncr = 1.0 / float(factor);
    for (int i = 1 ; i <= factor ; i++) {
        float alpha = i * alphaIncr;
        Mat warpFrame;
        inverseWarpSingle(rows, cols, flow, alpha, tri, prevFrame,
                          warpFrame);
        warpFrameVector.push_back(warpFrame);
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

void SloMo::slowdown(string const& inFilename, string const outFilename, const int factor)
{
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

    const int rows = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    const int cols = cap.get(CV_CAP_PROP_FRAME_WIDTH);

    VideoWriter vw(outFilename,
                   static_cast<int>(cap.get(CV_CAP_PROP_FOURCC)),
                   cap.get(CV_CAP_PROP_FPS),
                   Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT))
    );

    if (!vw.isOpened()) {
        cerr << "Could not open output file " << outFilename << endl << flush;
        return;
    }

    Mat flow, cflow, frame, prevframe, prevframeN;
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
        vector<Point2f> corners;
        Scalar meanVal = mean(gray);
        double minVal, maxVal;
        minMaxLoc(gray, &minVal, &maxVal);
        double cannyThreshold1 = std::max(0.33 * meanVal.val[0], 0.25 * maxVal);
        double cannyThreshold2 = std::min(1.33 * meanVal.val[0], 0.75 * maxVal);
        Canny(gray, edges, cannyThreshold1, cannyThreshold2, 3, true);
        goodFeaturesToTrack(gray, corners, maxCorners, qualityLevel, minDistance);


        // Display edges
        // Mat edgesD;
        // edges.convertTo(edgesD, CV_8UC1, 255, 0);
        // imshow("flow", edgesD);
        // cvWaitKey(0);


        // Display corners

//        Mat ccorner;
//        cvtColor(gray, ccorner, COLOR_GRAY2BGR);
//        for (int i = 0 ; i < corners.size() ; i++) {
//            circle(ccorner, corners[i], 2, Scalar(0,255,0), -1);
//        }
//        imshow("flow", ccorner);
//        cvWaitKey(0);

        Mat frameN;
        frame.convertTo(frameN, CV_32FC3, 1.0/255.0, 0);

        vector<Mat> wframe = vector<Mat>(factor, Mat::zeros(rows,cols,frame.type()));
        vector<Mat> wframeN;

        // if (firstFrame) {

            // Triangulate

        TIME_START(2)
        vector<vector<Point2f> > tri;
        // unordered_map<Point2i, int, std::Point2iHash> pointToTri;
        triangulate(frame.rows, frame.cols, blockSize, tri, /* pointToTri, */edges,corners);
        firstFrame = false;
        TIME_END(2, "Triangulation")

        if (firstFrame) {
            vw.write(frame);
        }


        if (!prevgray.empty()) {
            

#if 0
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
#else
            Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();
            tvl1->calc(prevgray, gray, uflow);
#endif


            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);

            // Do the warping
            prevframe.convertTo(prevframeN, CV_32FC3, 1.0/225.0, 0);
            inverseWarpAll(rows, cols, flow, factor, tri, prevframeN, wframeN);

            //for (int i = 0 ; i < factor ; i++) {
                //wframeN[i].convertTo(wframe[i], frame.type(), 255.0, 0);

                //for (int j = 0 ; j < int(corners.size()) ; j++) {
                //    circle(wframe[i], corners[j], 2, Scalar(0, 255, 0), -1);
                //}
                //vw.write(wframe[i]);
            //}

            const float alphaIncr = 1.0f / float(factor);
            for (int i = 1 ; i <= factor ; i++) {
                float alpha = i * alphaIncr;
                Mat iframeN = (1 - alpha) * wframeN[i-1] + alpha * frameN;
                Mat iframe;
                iframeN.convertTo(iframe, frame.type(), 255.0, 0);
                vw.write(iframe);

            }


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
            // vw.write(prevframe);
            // vw.write(wframe);

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