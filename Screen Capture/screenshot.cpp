#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;

using namespace cv;

void ImageFromDisplay(vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel) {
	Display* display = XOpenDisplay(nullptr);
	Window root = DefaultRootWindow(display);

	XWindowAttributes attributes = {0};
	XGetWindowAttributes(display, root, &attributes);

	Width = 300;
	Height = 300;

	XImage* img = XGetImage(display, root, 0, 0, Width, Height, AllPlanes, ZPixmap);
	BitsPerPixel = img -> bits_per_pixel;
	Pixels.resize(Width * Height * 4);

	memcpy(&Pixels[0], img -> data, Pixels.size());

	XDestroyImage(img);
	XCloseDisplay(display);
}

int main() {
	int Width = 0;
	int Height = 0;
	int Bpp = 0;
	vector<uint8_t> Pixels;
	namedWindow("WindowTitle", WINDOW_AUTOSIZE);
	for (int i = 0; i < 100; i++) {
		ImageFromDisplay(Pixels, Width, Height, Bpp);
		if (Width && Height) {
			Mat img = Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
				imshow("Display Window", img);
			}
		waitKey(0);
	}
	return 0;
}