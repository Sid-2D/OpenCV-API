// Detects a 2D grid in an image

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace std;
using namespace cv;

void drawLine(Vec2f it, Mat img, Scalar rgb = CV_RGB(0, 0, 255)) {
	if (it[1] != 0) {
		float m = -1 / tan(it[1]);
		float c = (it)[0] / sin(it[1]);
		line(img, Point(0, c), Point(img.size().width, m * img.size().width + c), rgb);
	} else {
		line(img, Point(it[0], 0), Point(it[0], img.size().height), rgb);
	}
}

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	Mat outerBox = Mat(image.size(), CV_8UC1);
	GaussianBlur(image, image, Size(11, 11), 0);
	adaptiveThreshold(image, outerBox, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 5, 2);
	bitwise_not(outerBox, outerBox);
	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0); 
	dilate(outerBox, outerBox, kernel);
	// Biggest blob
	int count = 0;
	int max = -1;
	Point maxPt;
	for (int i = 0; i < outerBox.size().height; i++) {
		uchar *row = outerBox.ptr(i);
		for (int j = 0; j < outerBox.size().width; j++) {
			if (row[j] >= 128) {
				int area = floodFill(outerBox, Point(j, i), CV_RGB(0, 0, 64));
				if (area > max) {
					maxPt = Point(j, i);
					max = area;
				}
			}
		}
	}
	floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));
	for (int i = 0; i < outerBox.size().height; i++) {
		uchar *row = outerBox.ptr(i);
		for (int j = 0; j < outerBox.size().width; j++) {
			if (row[j] == 64 && j != maxPt.x && i != maxPt.y) {
				int area = floodFill(outerBox, Point(j, i), CV_RGB(0, 0, 0));
			}
		}
	}
	erode(outerBox, outerBox, kernel);
	// Hough Lines
	vector<Vec2f> lines;
	HoughLines(outerBox, lines, 1, CV_PI/180, 200, 0, 0);
	vector<Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		drawLine(*it, outerBox, CV_RGB(0, 0, 128));
		++it;
	}
 	namedWindow("Operation");
	imshow("Operation", outerBox);
	waitKey(0);
	return 0;
}