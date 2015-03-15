// Test Code for opencv optical flow : adapted from source : https://github.com/Itseez/opencv/blob/master/samples/cpp/tvl1_optical_flow.cpp

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"

#include <iostream>
#include <cstring>
#include <string>
#include <cassert>

using namespace cv;
using namespace std;
using namespace cv::optflow;

static void help()
{
    cout <<
            "./fback <videofile>\n"
            "This reads from video file\n" << endl;
}
static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
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

int main(int argc, char* argv[])
{
    const string tmpDir = "tmp";

    if (argc != 2) {
	cerr << "Usage: fback <videofile>" << endl;
	return 1;
    }


    VideoCapture cap(argv[1]);
    // help();
    if( !cap.isOpened() )
        return -1;

    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    //namedWindow("flow", 1);
    int frameCtr = 0;

    for(;;)
    {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( !prevgray.empty() )
        {

            // Uncomment the required method

            // Method 1: Farneback
            // calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);

            // Method 2: DualTV-L1
            // Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();
            // tvl1->calc(prevgray, gray, uflow);

            // Method 3: Deep Flow
            Ptr<DenseOpticalFlow> dfl = createOptFlow_DeepFlow();
            dfl->calc(prevgray, gray, uflow);

            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);

            for (int i = 0 ; i < flow.rows ; i++) {
                for (int j = 0 ; j < flow.cols ; j++) {
                    Point2f pt = flow.at<Point2f>(Point(i,j));
                    assert(isnormal(pt.x));
                    assert(isnormal(pt.y));
                }
            }


            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            //imshow("flow", cflow);
            char flowOutFile[256];
            sprintf(flowOutFile, "%s/testflow_%04d.png", tmpDir.c_str(), frameCtr);
            imwrite(flowOutFile, cflow);
            cout << "Frame #" << frameCtr << endl << flush;
            frameCtr++;
        }
        // if(waitKey(30)>=0)
        //    break;
        std::swap(prevgray, gray);
    }
    return 0;
}
