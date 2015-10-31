#include "iostream"
#include <opencv2/opencv.hpp>
#include "cv.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv){
	Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY );

	cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("destination",CV_WINDOW_AUTOSIZE);

    Mat edges;
	Canny( gray, edges, 30, 210);
	edges =  cv::Scalar::all(255) - edges;

    Mat distanceMap;
	distanceTransform (edges, distanceMap, CV_DIST_L2, 5);

	vector<Mat> chan(3);

	split(image, chan);

    vector<Mat> integ(3);

    for (int i=0;i<3;i++){
	    integral (chan[i], integ[i], -1);
    }

	Point lt = Point (0,0);
	Point rb = Point (0,0);

    for (int numChan = 0; numChan<3;numChan++) {
	    for (int j = 0; j < gray.cols; j++) {
		    for (int i = 0; i < gray.rows; i++){

			    int dist = (int)(distanceMap.at<float>(i,j)/1.5);

			    lt.x = (i - dist) < 0 ? 0 : i - dist;
			    lt.y = (j - dist) < 0 ? 0 : j - dist;

			    rb.x = (i + dist + 1) > gray.rows ? gray.rows: (i + dist + 1);
			    rb.y = (j + dist + 1) > gray.cols ? gray.cols: j + dist + 1;

			    int sqare = (rb.y - lt.y)*(rb.x - lt.x);

			    int tlIntegr= (int)integ[numChan].at<int>(lt.x, lt.y);
			    int trIntegr= (int)integ[numChan].at<int>(lt.x, rb.y);
			    int blIntegr= (int)integ[numChan].at<int>(rb.x,lt.y);
			    int brIntegr= (int)integ[numChan].at<int>(rb.x,rb.y);

			    int newPixel = (int)((brIntegr-blIntegr-trIntegr+tlIntegr)/sqare);

			    chan[numChan].at<uchar>(i,j) = (uchar) newPixel;
		    }
	    }
    }

    Mat dst = Mat(image.rows, image.cols, image.type());
	cv::merge(chan, dst);

	imshow("original", image);
    imshow("destination", dst);

	waitKey(0);

	cvDestroyWindow("original");
	cvDestroyWindow("destination");

	return 0;
}
