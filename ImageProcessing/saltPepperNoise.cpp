// Adds random salt-pepper noise(white spots) on an image

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void salt(Mat &img, int n) {
	for (int k = 0; k < n; k++) {
		int i = rand() % img.rows;
		int j = rand() % img.cols;
		if (img.channels() == 1) {
			img.at<uchar>(i, j) = 255;
		} else if (img.channels() == 3) {
			img.at<Vec3b>(i, j)[0] = 255;
			img.at<Vec3b>(i, j)[1] = 255;
			img.at<Vec3b>(i, j)[2] = 255;
		}
	}
}

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/img.jpg");
	salt(image, 100000);
	resize(image, image, Size(), 0.5, 0.4, CV_INTER_LINEAR);
	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);
	return 0;
}