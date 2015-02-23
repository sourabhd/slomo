#ifndef SLOMO_SLOMO_HPP
#define SLOMO_SLOMO_HPP

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using std::string;
using std::vector;
using cv::Point2f;
using cv::Mat;

namespace slomo
{
    class SloMo
    {

    public:
        void slowdown(const string &inFilename, const string outFilename);
        void triangulate(const int rows, const int cols, const int blockSize,
                         vector<vector<Point2f> > &tri);
        Mat inverseAffine(vector<Point2f> &src, vector<Point2f> &dst);
        void inverseWarp(const Mat &flow, const vector<vector<Point2f> > &tri,
                         const Mat &prevFrame, Mat &warpFrame);
    };
}

#endif