// Inverts the colors of a Greyscale image

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	namedWindow("Original");
	imshow("Original", image);
	Mat result;
	int dim(256);
	Mat lookup(1, &dim, CV_8U);
	for (int i = 0; i < 256; i++) {
		lookup.at<uchar>(i) = 255 - i;
	}
	LUT(image, lookup, result);
	namedWindow("Inverted");
	imshow("Inverted", result);
	waitKey(0);
	return 0;
}