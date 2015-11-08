#include "iostream"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int argc, char** argv){

    Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	namedWindow("original",CV_WINDOW_AUTOSIZE);
	namedWindow("blur",CV_WINDOW_AUTOSIZE);
    namedWindow("equalizeHist",CV_WINDOW_AUTOSIZE);
    namedWindow("morphologyEx",CV_WINDOW_AUTOSIZE);

    Mat bluredImg;
    blur(image, bluredImg, Size(3, 3));

    Mat eHist;
    cvtColor( image, eHist, CV_BGR2GRAY );
    equalizeHist(eHist, eHist);

    Mat mEx;
    morphologyEx(image, mEx, MORPH_BLACKHAT, getStructuringElement(MORPH_RECT,Size(5,5)));

    imshow("original", image);
    imshow("blur", bluredImg);
    imshow("equalizeHist", eHist);
    imshow("morphologyEx", mEx);

    waitKey(0);

    return 0;
}