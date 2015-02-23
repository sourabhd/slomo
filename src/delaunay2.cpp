// Adapted from OpenCV samples at:
//  https://raw.githubusercontent.com/Itseez/opencv/master/samples/cpp/delaunay2.cpp


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include "delaunay2.hpp"

using namespace cv;
using namespace std;


static void draw_subdiv_point( Mat& img, Point2f fp, Scalar color )
{
    circle( img, fp, 3, color, FILLED, LINE_8, 0 );
}

static void draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
#if 1
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
        line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
        line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
    }
#else
    vector<Vec4f> edgeList;
    subdiv.getEdgeList(edgeList);
    for( size_t i = 0; i < edgeList.size(); i++ )
    {
        Vec4f e = edgeList[i];
        Point pt0 = Point(cvRound(e[0]), cvRound(e[1]));
        Point pt1 = Point(cvRound(e[2]), cvRound(e[3]));
        line(img, pt0, pt1, delaunay_color, 1, LINE_AA, 0);
    }
#endif
}

static void locate_point( Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar active_color )
{
    int e0=0, vertex=0;

    subdiv.locate(fp, e0, vertex);

    if( e0 > 0 )
    {
        int e = e0;
        do
        {
            Point2f org, dst;
            if( subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0 )
                line( img, org, dst, active_color, 3, LINE_AA, 0 );

            e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
        }
        while( e != e0 );
    }

    draw_subdiv_point( img, fp, active_color );
}


static void paint_voronoi( Mat& img, Subdiv2D& subdiv )
{
    vector<vector<Point2f> > facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];

        Scalar color;
        color[0] = rand() & 255;
        color[1] = rand() & 255;
        color[2] = rand() & 255;
        fillConvexPoly(img, ifacet, color, 8, 0);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, Scalar(), 1, LINE_AA, 0);
        circle(img, centers[i], 3, Scalar(), FILLED, LINE_AA, 0);
    }
}


//int main( int, char** )
int delaunay(const vector<Point2f> &pts, const int rows, const int cols,
             vector<vector<Point2f> > &tri)
{
    //help();

    cerr << "size: " << rows << " " << cols << endl;
    Scalar active_facet_color(0, 0, 255), delaunay_color(255,255,255);
    Rect rect(0, 0, rows, cols);

    Subdiv2D subdiv(rect);

    for( int i = 0; i < int(pts.size()); i++ ) {
        subdiv.insert(pts[i]);
    }

    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);

    for( size_t i = 0; i < triangleList.size(); i++ ) {
        vector<Point2f> pt(3);
        Vec6f t = triangleList[i];
        pt[0] = Point2f(t[0], t[1]);
        pt[1] = Point2f(t[2], t[3]);
        pt[2] = Point2f(t[4], t[5]);
        if ( \
            (pt[0].x >= 0 && pt[0].x < rows) && \
            (pt[1].x >= 0 && pt[1].x < rows) && \
            (pt[2].x >= 0 && pt[2].x < rows) && \
            (pt[0].y >= 0 && pt[0].y < cols) && \
            (pt[1].y >= 0 && pt[1].y < cols) && \
            (pt[2].y >= 0 && pt[2].y < cols)
                ) {
            // Ensure only valid values are returned
            // Opencv triangulation returns invalid values at borders

            tri.push_back(pt);
        }
    }


#if 0

    Mat img(rect.size(), CV_8UC3);

    img = Scalar::all(0);
    string win = "Delaunay Demo";
    imshow(win, img);

    img = Scalar::all(0);
    draw_subdiv( img, subdiv, delaunay_color );
    imshow( win, img );

    waitKey(0);

    img = Scalar::all(0);
    paint_voronoi( img, subdiv );
    imshow( win, img );

    waitKey(0);
    #endif

    return 0;
}
