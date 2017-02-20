// Converts a real time color video to grayscale.

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main() {
	Mat image, gray;
	char key = 0;
	VideoCapture capture(0);
	// Error handling
	if (!capture.isOpened()) {
		printf("Could not open video device!");
		return 1;
	}
	// Capture device properties
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	namedWindow("Camera Video", CV_WINDOW_AUTOSIZE);
	namedWindow("Processed Video", CV_WINDOW_AUTOSIZE);
	while (key != 'q') {
		// Get frame from camera
		capture >> image;
		cvtColor(image, gray, CV_BGR2GRAY);
		imshow("Camera Video", image);
		imshow("Processed Video", gray);
		key = waitKey(25);
	}
	return 0;
}