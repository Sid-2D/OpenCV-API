// Turn ON pixels of a specified color, uses strategy pattern and Iterators.

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class ColorDetector {
	private:
		int minDist;
		Vec3b target;
		Mat result;
		int getDistance(const Vec3b& color) const {
			return abs(color[0] - target[0]) + abs(color[1] - target[1]) + abs(color[2] - target[2]);
		}
	public:
		ColorDetector(): minDist(100) {
			target[0] = target[1] = target[2] = 0;
		}
		void setColorDistanceThreshold(int distance) {
			if (distance < 0) {
				distance = 0;
			}
			minDist = distance;
		}
		int getColorDistanceThreshold() const {
			return minDist;
		}
		void setTargetColor(unsigned char red, unsigned char green, unsigned char blue) {
			target[2] = red;
			target[1] = green;
			target[0] = blue;
		}
		void setTargetColor(Vec3b color) {
			target = color;
		}
		Vec3b getTargetColor() const {
			return target;
		}
		Mat process(const Mat &image) {
			result.create(image.rows, image.cols, CV_8U);
			Mat_<Vec3b>::const_iterator it = image.begin<Vec3b>();
			Mat_<Vec3b>::const_iterator itend = image.end<Vec3b>();
			Mat_<uchar>::iterator itout = result.begin<uchar>();
			for ( ; it!= itend; ++it, ++itout) {
				if (getDistance(*it)<minDist) {
					*itout= 255;
				} else {
					*itout= 0;
				}
			}
			return result;
		}
};

int main() {
	ColorDetector cdetect;
	Mat image = imread("C:/OCR/media/img.jpg");
	if (!image.data) {
		return 0;
	}
	cdetect.setTargetColor(100, 100, 200);
	namedWindow("Result");
	resize(image, image, Size(), 0.5, 0.4, CV_INTER_LINEAR);
	imshow("Result", cdetect.process(image));
	waitKey(0);
	return 0;
}