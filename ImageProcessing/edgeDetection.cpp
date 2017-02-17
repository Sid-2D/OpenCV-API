// Performs edge detection on an image

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {
	Mat image, gray, edge, cedge;
	int edgeThresh = 50;
	image = imread("C:/OCR/OpenCVApps/Media/tmb1.jpg");
	// Create single channel image for storing gray image
	gray.create(image.rows, image.cols, CV_8UC1);
	// Convert RGB to GrayScale
	cvtColor(image, gray, CV_BGR2GRAY);
	cedge.create(gray.size(), gray.type());
	// Create image to store edge detected image
	cedge.create(gray.size(), gray.type());
	Canny(gray, edge, edgeThresh, edgeThresh * 3, 3);
	cedge = Scalar::all(0);
	image.copyTo(cedge, edge);
	namedWindow("Output Image", CV_WINDOW_AUTOSIZE);
	resize(cedge, cedge, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	imshow("Output Image", cedge);
	waitKey(0);
	return 0;
}