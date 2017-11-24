// Detects a 2D grid in an image
// Referred heavily from: http://aishack.in/tutorials/sudoku-grabber-opencv-detection/

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>

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
	Mat image = imread("/home/sid/Desktop/OpenCV-API/Media/tmbSharpened.jpg", 0);
	Mat original = imread("/home/sid/Desktop/OpenCV-API/Media/tmbSharpened.jpg", 0);
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
 	// namedWindow("HoughLines OuterBox");
	// imshow("HoughLines OuterBox", outerBox);
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
	// namedWindow("Original with lines");
	// imshow("Original with lines", original);
	// Calculate Line Intersections
	Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;
	int height = outerBox.size().height;
	int width = outerBox.size().width;
	cout << "rightedge0: " << rightEdge[0] << endl;
	cout << "rightedge1: " << rightEdge[1] << endl;
	// if (leftEdge[1] != 0) {
		// left1.x = 0;
		// left1.y = leftEdge[0] / sin(leftEdge[1]);
		// left2.x = width;
		// left2.y = -left2.x / tan(leftEdge[1]) + left1.y;
	//} else {
		left1.y = 0;
		left1.x = leftEdge[0] / cos(leftEdge[1]);
		left2.y = height;
		left2.x = left1.x - height * tan(leftEdge[1]);
	// }
	// if (rightEdge[1] != 0) {
		// right1.x = 0;
		// right1.y = rightEdge[0] / sin(rightEdge[1]);
		// right2.x = width;
		// right2.y = -right2.x / tan(rightEdge[1]) + right1.y; 
	// } else {
		right1.y = 0;
		right1.x = rightEdge[0] / cos(rightEdge[1]);
		right2.y = height;
		right2.x = right1.x - height * tan(rightEdge[1]);
	// }
	bottom1.x = 0;
	bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);
	bottom2.x = width;
	bottom2.y = -bottom2.x / tan(bottomEdge[1]) + bottom1.y;
	top1.x = 0;
	top1.y = topEdge[0] / sin(topEdge[1]);
	top2.x = width;
	top2.y = -top2.x / tan(topEdge[1]) + top1.y;
	// Display above points
	cout << "Height: " << height << endl;
	cout << "Width: " << width << endl;
	cout << "Left1 x:" << left1.x << endl;
	cout << "Left1 y:" << left1.y << endl;
	cout << "Left2 x:" << left2.x << endl;
	cout << "Left2 y:" << left2.y << endl;
	cout << "Right1 x:" << right1.x << endl;
	cout << "Right1 y:" << right1.y << endl;
	cout << "Right2 x:" << right2.x << endl;
	cout << "Right2 y:" << right2.y << endl;
	circle(original, left1, 50, CV_RGB(0, 0, 255));
	circle(original, left2, 50, CV_RGB(0, 0, 255));
	circle(original, right1, 50, CV_RGB(0, 0, 255));
	circle(original, right2, 50, CV_RGB(0, 0, 255));
	circle(original, top1, 50, CV_RGB(0, 0, 255));
	circle(original, top2, 50, CV_RGB(0, 0, 255));
	circle(original, bottom1, 50, CV_RGB(0, 0, 255));
	circle(original, bottom2, 50, CV_RGB(0, 0, 255));
	Mat resized;
	resize(original, resized, Size(), 0.5, 0.4, CV_INTER_LINEAR);
	namedWindow("Points");
	imshow("Points", resized);

    // Next, we find the intersection of  these four lines
    double leftA = left2.y-left1.y;
    double leftB = left1.x-left2.x;

    double leftC = leftA*left1.x + leftB*left1.y;

    double rightA = right2.y-right1.y;
    double rightB = right1.x-right2.x;

    double rightC = rightA*right1.x + rightB*right1.y;

    double topA = top2.y-top1.y;
    double topB = top1.x-top2.x;

    double topC = topA*top1.x + topB*top1.y;

    double bottomA = bottom2.y-bottom1.y;
    double bottomB = bottom1.x-bottom2.x;

    double bottomC = bottomA*bottom1.x + bottomB*bottom1.y;

    // Intersection of left and top
    double detTopLeft = leftA*topB - leftB*topA;

    CvPoint ptTopLeft = cvPoint((topB*leftC - leftB*topC)/detTopLeft, (leftA*topC - topA*leftC)/detTopLeft);

    // Intersection of top and right
    double detTopRight = rightA*topB - rightB*topA;

    CvPoint ptTopRight = cvPoint((topB*rightC-rightB*topC)/detTopRight, (rightA*topC-topA*rightC)/detTopRight);

    // Intersection of right and bottom
    double detBottomRight = rightA*bottomB - rightB*bottomA;
    CvPoint ptBottomRight = cvPoint((bottomB*rightC-rightB*bottomC)/detBottomRight, (rightA*bottomC-bottomA*rightC)/detBottomRight);// Intersection of bottom and left
    double detBottomLeft = leftA*bottomB-leftB*bottomA;
    CvPoint ptBottomLeft = cvPoint((bottomB*leftC-leftB*bottomC)/detBottomLeft, (leftA*bottomC-bottomA*leftC)/detBottomLeft);

	int maxLength = (ptBottomLeft.x-ptBottomRight.x)*(ptBottomLeft.x-ptBottomRight.x) + (ptBottomLeft.y-ptBottomRight.y)*(ptBottomLeft.y-ptBottomRight.y);
    int temp = (ptTopRight.x-ptBottomRight.x)*(ptTopRight.x-ptBottomRight.x) + (ptTopRight.y-ptBottomRight.y)*(ptTopRight.y-ptBottomRight.y);

    if(temp>maxLength) maxLength = temp;

    temp = (ptTopRight.x-ptTopLeft.x)*(ptTopRight.x-ptTopLeft.x) + (ptTopRight.y-ptTopLeft.y)*(ptTopRight.y-ptTopLeft.y);

    if(temp>maxLength) maxLength = temp;

    temp = (ptBottomLeft.x-ptTopLeft.x)*(ptBottomLeft.x-ptTopLeft.x) + (ptBottomLeft.y-ptTopLeft.y)*(ptBottomLeft.y-ptTopLeft.y);

    if(temp>maxLength) maxLength = temp;

    maxLength = sqrt((double)maxLength);

	Point2f src[4], dst[4];
	src[0] = ptTopLeft;            dst[0] = Point2f(0,0);
	src[1] = ptTopRight;        dst[1] = Point2f(maxLength-1, 0);
	src[2] = ptBottomRight;        dst[2] = Point2f(maxLength-1, maxLength-1);
	src[3] = ptBottomLeft;        dst[3] = Point2f(0, maxLength-1);

	Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
	cv::warpPerspective(original, undistorted, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));

	resize(undistorted, resized, Size(), 0.4, 0.28, CV_INTER_LINEAR);

	namedWindow("Image Cut");
	imshow("Image Cut", resized);

	waitKey(0);
	return 0;
}