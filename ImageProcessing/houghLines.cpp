// Uses HoughLines operator to draw lines on an image.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	Mat contours;
	Canny(image, contours, 120, 360);
	namedWindow("Canny Contours");
	imshow("Canny Contours", contours);
	vector<Vec2f> lines;
	HoughLines(contours, lines, 1, CV_PI/180, 300, 0, 0);
	vector<Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0];
		float theta = (*it)[1];
	    Point pt1, pt2;
	    double a = cos(theta), b = sin(theta);
	    double x0 = a * rho, y0 = b * rho;
	    pt1.x = (x0 + 1000 * (-b));
	    pt1.y = (y0 + 1000 * (a));
	    pt2.x = (x0 - 1000 * (-b));
	    pt2.y = (y0 - 1000 * (a));
	    line(contours, pt1, pt2, Scalar(255), 2, CV_AA);
		++it;
	}
	namedWindow("Hough Lines");
	imshow("Hough Lines", contours);
	waitKey(0);
	return 0;
}