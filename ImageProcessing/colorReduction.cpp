// Reduces the number of colors in an image

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void colorReduce(Mat &img, int div) {
	int rows = img.rows;
	int cols = img.cols * img.channels();
	for (int i = 0; i < rows; i++) {
		uchar *data = img.ptr<uchar>(i);
		for (int j = 0; j < cols; j++) {
			data[j] =  (data[j] / div) * div + div / 2;
		}
	}
}

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/img.jpg");
	colorReduce(image, 64);
	resize(image, image, Size(), 0.5, 0.4, CV_INTER_LINEAR);
	namedWindow("Image");
	imshow("Image", image);
	waitKey(0);
	return 0;
}