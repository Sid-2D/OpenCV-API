// Reads an image, resizes it and converts it to grayscale.

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void thresholding(Mat &img, uchar threshold) {
	int numRows = img.rows;
	int numCols = img.cols;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			if (img.channels() == 1) {
				// Grayscale Image
				uchar tValue = img.at<uchar>(i, j);
				if (tValue > threshold) {
					img.at<uchar>(i, j) = 255;
				}
			} else if (img.channels() == 3) {
				// Color Image
				int tSum = img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i, j)[1] + img.at<Vec3b>(i, j)[2];
				uchar averageValue = static_cast<uchar>(tSum / 3);
				if (averageValue > threshold) {
					img.at<Vec3b>(i, j)[0] = 255;	// B
					img.at<Vec3b>(i, j)[1] = 255;	// G
					img.at<Vec3b>(i, j)[2] = 255;	// R
				} else {
					img.at<Vec3b>(i, j)[0] = 0;	// B
					img.at<Vec3b>(i, j)[1] = 0;	// G
					img.at<Vec3b>(i, j)[2] = 0;	// R
				}
 			}
		}
	}
}

int main (int argc, char** argv) {
	Mat org, color, grayscale;
	org = imread("C:/OCR/OpenCVApps/Media/tmb1.jpg");
	resize(org, color, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	namedWindow("Color Image", CV_WINDOW_AUTOSIZE);
	imshow("Color Image", color);
	thresholding(org, 150);
	resize(org, grayscale, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	namedWindow("Grayscale Image", CV_WINDOW_AUTOSIZE);
	imshow("Grayscale Image", grayscale);
	waitKey(0);
	return 0;
}