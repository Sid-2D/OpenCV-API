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

struct ScreenShot {

    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;
    bool init;

    ScreenShot(int x, int y, int width, int height): x(x), y(y), width(width), height(height) {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        init = true;
    }

    void operator() (Mat& cvImg) {
        if(init == true) {
            init = false;
        }
        else {
            XDestroyImage(img);
        }
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        cvImg = Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot() {
        if(init == false) {
            XDestroyImage(img);
        }
        XCloseDisplay(display);
    }
};


int main() {
    for (;;) {
        ScreenShot screen(0, 0, 300, 300);
        Mat img;
        screen(img);
        imshow("img", img);
        // 'img' contains an image frame
        if (waitKey(25) >= 0) {
            continue;
        }
    }
    return 0;
}