// Get individual blocks from processed image of gridDetection.cpp

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("../Media/grid.jpg");
	Mat scaled;
	resize(img, scaled, Size(), 0.5, 0.28, CV_INTER_LINEAR);
	namedWindow("Grid");
	imshow("Grid", scaled);
	// Start extraction
	int numberHeight = scaled.size().height / 3;
	int numberWidth = scaled.size().width / 9;
	vector<Mat> numbers;
	int totalNumbers = 0;
	int startX = 0, startY = 0;
	while (totalNumbers < 27) {
		Rect rect = Rect(startX, startY, numberWidth, numberHeight);
		totalNumbers++;
		if (totalNumbers % 9 == 0) {
			startX = 0;
			startY += numberHeight; 
		} else {
			startX += numberWidth;
		}
		numbers.push_back(Mat(scaled, rect));
	}
	for (int i = 0; i < numbers.size(); i++) {
		// namedWindow("Numbers", CV_WINDOW_AUTOSIZE);
		// imshow("Numbers", numbers[i]);
		char name[10];
		sprintf(name, "../Media/Numbers/%d.jpg", i);
		imwrite(name, numbers[i]);
	}
	return 0;
}