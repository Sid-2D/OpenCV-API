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
		float c = it[0] / sin(it[1]);
		line(img, Point(0, c), Point(img.size().width, m * img.size().width + c), rgb);
	} else {
		line(img, Point(it[0], 0), Point(it[0], img.size().height), rgb);
	}
}

void mergeRelatedLines(vector<Vec2f> &lines, Mat &img) {
	vector<Vec2f>::iterator it;
	for (it = lines.begin(); it != lines.end(); it++) {
		if ((*it)[0] == 0 && (*it)[1] == -100) {
			continue;
		}
		float p1 = (*it)[0];
		float theta1 = (*it)[1];
		Point pt1Current, pt2Current;
		if (theta1 > CV_PI * 45 / 180 && theta1 < CV_PI * 135 / 180) {
			pt1Current.x = 0;
			pt1Current.y = p1 / sin(theta1);
			pt2Current.x = img.size().width;
			pt2Current.y = -pt2Current.x / tan(theta1) + p1 / sin(theta1);
		} else {
			pt1Current.y = 0;
			pt1Current.x = p1 / cos(theta1);
			pt2Current.y = img.size().height;
			pt2Current.x = -pt2Current.y / tan(theta1) + p1 / cos(theta1);
		}
		vector<Vec2f>::iterator pos;
		for (pos = lines.begin(); pos != lines.end(); pos++) {
			if (*it == *pos) {
				continue;
			}
			if (fabs((*pos)[0] - (*it)[0]) < 20 && fabs((*pos)[1] - (*it)[1]) < CV_PI * 10 / 180) {
				float p = (*pos)[0];
				float theta = (*pos)[1];
				Point pt1, pt2;
				if ((*pos)[0] > CV_PI * 45 / 180 && (*pos)[1] < CV_PI * 135 / 180) {
					pt1.x = 0;
					pt1.y = p / sin(theta);
					pt2.x = img.size().width;
					pt2.y = -pt2.x / tan(theta) + p / sin(theta);
				} else {
					pt1.y = 0;
					pt1.x = p / cos(theta);
					pt2.y = img.size().height;
					pt2.x = -pt2.y / tan(theta) + p / cos(theta);
				}
				if (((double)(pt1.x - pt1Current.x) * (pt1.x - pt1Current.x) + (pt1.y - pt1Current.y) * (pt1.y - pt1Current.y) < 64 * 64) && ((double)(pt2.x - pt2Current.x) * (pt2.x - pt2Current.x) + (pt2.y - pt2Current.y) * (pt2.y - pt2Current.y) < 64 * 64)) {
					(*it)[0] = ((*it)[0] + (*pos)[0]) / 2;
					(*it)[1] = ((*it)[1] + (*pos)[1]) / 2;
					(*pos)[0] = 0;
					(*pos)[1] = -100;
				}
			}
		}
	}
}

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	Mat original = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
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
	// Hough Lines and merge
	vector<Vec2f> lines;
	HoughLines(outerBox, lines, 1, CV_PI/180, 200, 0, 0);
	mergeRelatedLines(lines, outerBox);
	vector<Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		drawLine(*it, outerBox, CV_RGB(0, 0, 128));
		++it;
	}
	// Finding extreme lines
	Vec2f topEdge = Vec2f(1000, 1000);
	double topYIntercept = 100000, topXIntercept = 0;
	Vec2f bottomEdge = Vec2f(-1000, -1000);
	double bottomYIntercept = 0, bottomXIntercept = 0;
	Vec2f leftEdge = Vec2f(1000, 1000);
	double leftXIntercept = 100000, leftYIntercept = 0;
	Vec2f rightEdge = Vec2f(-1000, -1000);
	double rightXIntercept = 0, rightYIntercept = 0;
	for (int i = 0; i < lines.size(); i++) {
		Vec2f current = lines[i];
		float p = current[0];
		float theta = current[1];
		if (p == 0 && theta == -100) {
			continue;
		}
		double xIntercept, yIntercept;
		xIntercept = p / cos(theta);
		yIntercept = p / (cos(theta) * sin(theta));
		// Check for vertical lines
		if (theta > CV_PI * 80 / 180 && theta < CV_PI * 100 / 180) {
			if (p < topEdge[0]) {
				topEdge = current;
			}
			if (p > bottomEdge[0]) {
				bottomEdge = current;
			}
		} else if (theta < CV_PI * 10 / 180 || theta > CV_PI * 170 / 180) {
			if (xIntercept > rightXIntercept) {
				rightEdge = current;
				rightXIntercept = xIntercept;
			} else if (xIntercept <= leftXIntercept) {
				leftEdge = current;
				leftXIntercept = xIntercept;
			}
		}
	}
	drawLine(topEdge, original, CV_RGB(0, 0, 0));
	drawLine(bottomEdge, original, CV_RGB(0, 0, 0));
	drawLine(leftEdge, original, CV_RGB(0, 0, 0));
	drawLine(rightEdge, original, CV_RGB(0, 0, 0));
	namedWindow("Original with lines");
	imshow("Original with lines", original);
 	namedWindow("Operation");
	imshow("Operation", outerBox);
	waitKey(0);
	return 0;
}