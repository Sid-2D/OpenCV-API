// Performs sharpening on an image after converting it to grayscale.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void sharpen(const Mat &image) {
	Mat result;
	result.create(image.size(), image.type());
	for (int i = 1; i < image.rows - 1; i++) {
		const uchar *previous = image.ptr<const uchar>(i - 1);
		const uchar *current = image.ptr<const uchar>(i);
		const uchar *next = image.ptr<const uchar>(i + 1);
		uchar *output = result.ptr<uchar>(i);
		for (int j = 1; j < image.cols - 1; j++) {
			*output++ = saturate_cast<uchar>(5 * current[j] - current[j - 1] - current[j + 1] - previous[j] - next[j]);
		}
	}
	result.row(0).setTo(Scalar(0));
	result.row(result.rows - 1).setTo(Scalar(0));
	result.col(0).setTo(Scalar(0));
	result.col(result.cols - 1).setTo(Scalar(0));
	namedWindow("Sharpened Image", CV_WINDOW_AUTOSIZE);
	imshow("Sharpened Image", result);
}

void sharpenUsingKernel(const Mat &image) {
	Mat result, kernel(3, 3, CV_32F, Scalar(0));
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = 2.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	filter2D(image, result, image.depth(), kernel);
	namedWindow("Sharpened Image", CV_WINDOW_AUTOSIZE);
	imshow("Sharpened Image", result);
}

int main() {
	Mat image, gray;
	image = imread("C:/OCR/Media/img.jpg");
	gray.create(image.rows, image.cols, CV_8UC1);
	cvtColor(image, gray, CV_BGR2GRAY);
	resize(gray, gray, Size(), 0.5, 0.4, INTER_LINEAR);
	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Image", gray);
	sharpenUsingKernel(gray);
	waitKey(0);
	return 0;
}