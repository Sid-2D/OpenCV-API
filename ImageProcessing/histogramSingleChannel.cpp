// Computes the histogram of a Single channel image.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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
};

int main() {
	Mat image = imread("C:/OCR/OpenCVApps/Media/tmbSharpened.jpg", 0);
	Histogram1D h;
	namedWindow("Histogram");
	imshow("Histogram", h.getHistogramImage(image));
	waitKey(0);
	return 0;
}