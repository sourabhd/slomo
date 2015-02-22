#ifndef SLOMO_DELAUNAY_HPP
#define SLOMO_DELAUNAY_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>


using cv::Point2f;
using std::vector;

int delaunay(const vector<Point2f> &pts, const int rows, const int cols,
             vector<vector<Point2f> > &facets, vector<Point2f> &centers);

#endif