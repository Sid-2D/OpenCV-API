// Finds contours in an image.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	Mat contours;
	Canny(image, contours, 120, 360);
	namedWindow("Canny Contours");
	imshow("Canny Contours", contours);
	Mat contoursInv;
	threshold(contours,contoursInv, 128, 255, THRESH_BINARY_INV);
	namedWindow("Inverted");
	imshow("Inverted", contoursInv);
	waitKey(0);
}