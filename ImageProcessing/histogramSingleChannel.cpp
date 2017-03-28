// Computes the histogram of a Single channel image.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class Histogram1D {
	private: 
		int histSize[1];
		float hranges[2];
		const float* ranges[1];
		int channels[1];
	public:
		Histogram1D() {
			histSize[0] = 256;
			hranges[0] = 0.0;
			hranges[1] = 255.0;
			ranges[0] = hranges;
			channels[0] = 0;
		}
		MatND getHistogram(const Mat &image) {
			MatND hist;
			calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
			return hist;
		}
		Mat getHistogramImage(const Mat &image) {
			MatND hist = getHistogram(image);
			double maxVal = 0;
			double minVal = 0;
			minMaxLoc(hist, &minVal, &maxVal, 0, 0);
			Mat histImg(histSize[0], histSize[0], CV_8U, Scalar(255));
			int hpt = static_cast<int>(0.9 * histSize[0]);
			for (int i = 0; i < histSize[0]; i++) {
				float binVal = hist.at<float>(i);
				int intensity = static_cast<int>(binVal * hpt / maxVal);
				line(histImg, Point(i, histSize[0]), Point(i, histSize[0] - intensity), Scalar::all(0));
			}
			return histImg;
		}
		Mat stretch(const Mat &image, int minValue = 0) {
			MatND hist = getHistogram(image);
			int imin;
			cout << "\nStarting Stretch:\n";
			cout << hist.at<float>(imin) << endl;
			for (imin = 0; imin < histSize[0]; imin++) {
				cout << hist.at<float>(imin) << endl;
				if (hist.at<float>(imin) > minValue) {
					break;
				}
			}
			cout << "imin: " << imin << endl;
			int imax;
			for (imax = histSize[0] - 1; imax >= 0; imax--) {
				if (hist.at<float>(imax) > minValue) {
					break;
				}
			}
			cout << "imax: " << imax << endl;
			int dim(256);
			Mat lookup(1, &dim, CV_8U);
			for (int i = 0; i < 256; i++) {
				if (i < imin) {
					lookup.at<uchar>(i) = 0;
				} else if (i > imax) {
					lookup.at<uchar>(i) = 255; 
				} else {
					lookup.at<uchar>(i) = static_cast<uchar>(255.0 * (i - imin) / (imax - imin) + 0.5);
				}
			}
			Mat result;
			LUT(image, lookup, result);
			return result;
		}
};

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	namedWindow("Original");
	imshow("Original", image);
	Histogram1D h;
	namedWindow("Histogram");
	imshow("Histogram", h.getHistogramImage(image));
	// Thresholding
	Mat thresholded;
	threshold(image, thresholded, 100, 255, THRESH_BINARY);
	// Stretching
	MatND histoStretched = h.getHistogramImage(h.stretch(image, 5000));
	namedWindow("Stretched Histogram");
	imshow("Stretched Histogram", histoStretched);
	namedWindow("Stretched Image");
	imshow("Stretched Image", h.stretch(image, 100));
	waitKey(0);
	return 0;
}