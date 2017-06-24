#include <X11/Xlib.h>  
#include <X11/Xutil.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

using namespace cv;

struct Screenshot {
	Display* display;
	Window root;
	int x, y, width, height;
	XImage* img;
	bool init;

	Screenshot(int x, int y, int width, int height): x(x), y(y), width(width), height(height) {
		display = XOpenDisplay(nullptr);
		root = DefaultRootWindow(display);
		init = true;
	}

	void operator() (Mat& cvImg) {
		if (init == true) {
			init = false;
		} else {
			XDestroyImage(img);
		}
		img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
		cvImg = Mat(height, width, CV_8UC4, img -> data);		
	}

	~Screenshot() {
		if (init == false) {
			XDestroyImage(img);
		}
		XCloseDisplay(display);
	}
};

void process(Mat &cvImg) {
	cvtColor(cvImg, cvImg, CV_BGR2GRAY);
	Canny(cvImg, cvImg, 120, 360);
}

int main() {
	while (true) {
		Screenshot screen(0, 0, 300, 300);
		Mat img;
		screen(img);
		process(img);
		imshow("img", img);
		if (waitKey(1000 / 60) >= 0) {
			continue;
		}
	}
	return 0;
}