/* calculates histogram of a grayscale image */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;

class ColorHistogram {
private:
	int histSize[3];		// number of bins
	float hranges[2];		// min and max pixel value
	const float* ranges[3];
	int channels[3]; 		// only 1 channel used here

public:
	ColorHistogram() {
		// prepare arguments
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0; hranges[1] = 255.0;
		ranges[0] = ranges[1] = ranges[2] = hranges;
		channels[0] = 0;	// the three channels
		channels[1] = 1;
		channels[2] = 2;
	}
	// calc histogram for an image
	cv::MatND getHistogram(const cv::Mat &image) {
		cv::MatND hist;

		// Compute histogram
		cv::calcHist(&image,
			1,				// histogram from 1 image only
			channels,		// channels used
			cv::Mat(),		// no mask is used
			hist,			// resulting histogram
			3,				// its a 3D histogram
			histSize,		// no of bins
			ranges			// pixel value range
		);

		return hist;
	}
};