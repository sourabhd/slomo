#ifndef SLOMO_SLOMO_HPP
#define SLOMO_SLOMO_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <opencv2/opencv.hpp>

using std::string;
using std::vector;
using cv::Point2f;
using cv::Point2i;
using cv::Mat;
using std::unordered_map;
using cv::VideoCapture;

namespace std {

    //template <class T>
    //class Point2iHash {};


    class Point2iHash
    {
    public:
        std::size_t operator()(cv::Point2i const &s) const {
            return std::hash<unsigned long long>()( \
                                                  (((unsigned long long) s.x) << 32) + \
                                                  (unsigned long long) s.y \
                                                  );
        }
    };
}

namespace slomo
{

    const int blockSize = 64;

    class SloMo
    {

    public:
        void slowdown(const string &inFilename, const string outFilename, const int factor);
        void triangulate(const int rows, const int cols, const int blockSize,
                         vector<vector<Point2f> > &tri,
                         unordered_map<Point2i, int, std::Point2iHash > &pointToTri,Mat &edges);
        Mat inverseAffine(vector<Point2f> &src, vector<Point2f> &dst);
        void inverseWarpSingle(const int rows, const int cols,
                               const Mat &flow, const double alpha,
                               const vector<vector<Point2f> > &tri,
                               const Mat &prevFrame, Mat &warpFrame
                               );
        void inverseWarpAll(const int rows, const int cols,
                            const Mat &flow, const int factor,
                            const vector<vector<Point2f> > &tri,
                            const Mat &prevFrame,
                            vector<Mat> &warpFrame
                           );
        void dumpVideoProp(VideoCapture &cap);
    };
}

#endif