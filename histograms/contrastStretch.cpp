/* streches the contast of an image */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;

class Histogram1D {
protected:
	int histSize[1];		// number of bins
	float hranges[2];		// min and max pixel value
	const float* ranges[1];
	int channels[1]; 		// only 1 channel used here

public:
	Histogram1D() {
		// prepare arguments
		histSize[0] = 256;
		hranges[0] = 0.0; hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0;	// look at channel 0
	}
	// calc histogram for an image
	cv::MatND getHistogram(const cv::Mat &image) {
		cv::MatND hist;

		// Compute histogram
		cv::calcHist(&image,
			1,				// histogram from 1 image only
			channels,		// channel used
			cv::Mat(),		// no mask is used
			hist,			// resulting histogram
			1,				// its a 1D histogram
			histSize,		// no of bins
			ranges			// pixel value range
		);

		return hist;
	}
	// calc histogram & returns an image of it
	cv::Mat getHistogramImage(const cv::Mat &image) {
		// compute histogram
		cv::MatND hist = getHistogram(image);

		// get min and max bin values
		double minVal = 0;
		double maxVal = 0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		// image on which to display hist
		cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));

		// set highest point at 90% of the nbins
		int hpt = static_cast<int>(0.9 * histSize[0]);

		// draw vertical line for each bin
		for(int h=0; h<histSize[0]; h++) {
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal * hpt/maxVal);

			// this function draws a line btw two points
			cv::line(histImg, cv::Point(h, histSize[0]),
								cv::Point(h, histSize[0]-intensity),
								cv::Scalar::all(0));
		}

		return histImg;
	}
};

class ContrastStretch {
private:
	Histogram1D h;
public:
	ContrastStretch() {}
	cv::Mat stretch(const cv::Mat &image, int minVal = 0) {
		// Compute histogram
		cv::MatND hist = h.getHistogram(image);

		// find imin
		int imin = 0;
		for(; imin< 256; imin++) {
			if(hist.at<float>(imin) > minVal)
				break;
		}
		cout << "imin, " << imin << endl;

		// find imax
		int imax = 255;
		for(; imax >= 0; imax--) {
			if(hist.at<float>(imax) > minVal)
				break;
		}
		cout << "imax, " << imax << endl;

		// Create lookup table
		int dim(256);
		cv::Mat lookup(1,
				&dim,
				CV_8U);
		// build lookup table
		for(int i=0; i<256; i++) {
			// stretch between imin and imax
			if(i < imin) lookup.at<uchar>(i) = 0;
			else if (i > imax) lookup.at<uchar>(i) = 255;
			// linear mapping
			else lookup.at<uchar>(i) = static_cast<uchar>( 255.0 * (i-imin)/(imax-imin)+0.5 );
		}

		// apply lut
		cv::Mat result;
		cv::LUT(image, lookup, result);
		return result;
	}
};

int main(int argc, char ** argv) {
	cv::Mat img = cv::imread(argv[1]);

	ContrastStretch contrast;
	cv::Mat stretched = contrast.stretch(img, 100);

	Histogram1D hist;
	cv::namedWindow("Histogram");
	cv::imshow("Histogram", hist.getHistogramImage(stretched));

	cv::namedWindow("Contrast Corrected");
	cv::imshow("Contrast Corrected", stretched);
	cv::waitKey();

	return 0;
}