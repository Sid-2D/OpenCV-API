// Performs steganography on two images

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void steganograph(Mat &front, Mat &hidden, Mat &stegedImage) {
	if (front.type() != hidden.type() || front.size() != hidden.size()) {
		printf("Given images are different \n");
		return;
	}
	stegedImage.create(front.rows, front.cols, front.type());
	Mat tFront, tHidden;
	Mat frontMask(front.rows, front.cols, front.type(), Scalar(0xF0, 0xF0, 0xF0));
	Mat hiddenMask(front.rows, front.cols, front.type(), Scalar(0xF0, 0xF0, 0xF0));
	bitwise_and(front, frontMask, tFront);
	bitwise_and(hidden, hiddenMask, tHidden);
	for (int i = 0; i < front.rows; i++) {
		for (int j = 0; j < front.cols; j++) {
			tHidden.at<Vec3b>(i, j)[0] = tHidden.at<Vec3b>(i, j)[0] >> 4;
			tHidden.at<Vec3b>(i, j)[1] = tHidden.at<Vec3b>(i, j)[1] >> 4;
			tHidden.at<Vec3b>(i, j)[2] = tHidden.at<Vec3b>(i, j)[2] >> 4;
		}
	}
	bitwise_or(tFront, tHidden, stegedImage);
}

void decodeSteganograph(Mat &stegedImage) {
	Mat frontImage, hiddenImage;
	frontImage.create(stegedImage.rows, stegedImage.cols, stegedImage.type());
	hiddenImage.create(stegedImage.rows, stegedImage.cols, stegedImage.type());
	Mat frontMask(stegedImage.rows, stegedImage.cols, stegedImage.type(), Scalar(0xF0, 0xF0, 0xF0));
	Mat hiddenMask(stegedImage.rows, stegedImage.cols, stegedImage.type(), Scalar(0x0F, 0x0F, 0x0F));
	bitwise_and(stegedImage, frontMask, frontImage);
	bitwise_and(stegedImage, hiddenMask, hiddenImage);
	for (int i = 0; i < hiddenImage.rows; i++) {
		for (int j = 0; j < hiddenImage.cols; j++) {
			hiddenImage.at<Vec3b>(i, j)[0] = hiddenImage.at<Vec3b>(i, j)[0] << 4;
			hiddenImage.at<Vec3b>(i, j)[1] = hiddenImage.at<Vec3b>(i, j)[1] << 4;
			hiddenImage.at<Vec3b>(i, j)[2] = hiddenImage.at<Vec3b>(i, j)[2] << 4;
 		}
	}
	namedWindow("Decoded Front Image", CV_WINDOW_AUTOSIZE);
	imshow("Decoded Front Image", frontImage);
	namedWindow("Decoded Hidden Image", CV_WINDOW_AUTOSIZE);
	imshow("Decoded Hidden Image", hiddenImage);
}

int main() {
	Mat hiddenImage, frontImage, stegedImage;
	hiddenImage = imread("C:/OCR/OpenCVApps/Media/tmb2.jpg");
	frontImage = imread("C:/OCR/OpenCVApps/Media/tmb3.jpg");
	steganograph(frontImage, hiddenImage, stegedImage);
	resize(stegedImage, stegedImage, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	resize(hiddenImage, hiddenImage, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	resize(frontImage, frontImage, Size(), 0.3, 0.28, CV_INTER_LINEAR);
	namedWindow("Original Front Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Front Image", frontImage);
	namedWindow("Original Hidden Image", CV_WINDOW_AUTOSIZE);
	imshow("Original Hidden Image", hiddenImage);
	namedWindow("Steged Image", CV_WINDOW_AUTOSIZE);
	imshow("Steged Image", stegedImage);
	decodeSteganograph(stegedImage);
	waitKey(0);
	return 0;
}