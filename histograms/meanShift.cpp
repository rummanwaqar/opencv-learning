/* identify baboon's face */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

class ColorHistogram {
private:
	int histSize[3];		// number of bins
	float hranges[2];		// min and max pixel value
	const float* ranges[3];
	int channels[3]; 

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
	// color reduce
	cv::Mat colorReduce(const cv::Mat &img, int n = 64) {
	  // no of lines
	  int nl = img.rows;
	  // elements per line
	  int nc = img.cols * img.channels();

	  cv::Mat result;
	  result.create(img.size(), img.type());

	  for(int j=0; j<nl; j++) {
	    const uchar* data_in = img.ptr<uchar>(j);
	    uchar* data_out = result.ptr<uchar>(j);
	    for(int i=0; i<nc; i++) {
	      data_out[i] = data_in[i]/n*n + n/2;
	    }
	  }
	  return result;
	}
	// computes the 1D hue histogram with mask
	cv::MatND getHueHistogram(const cv::Mat &image, int minSaturation = 0) {
		cv::MatND hist;

		cv::Mat hsv;

		// convert BGR to HSV
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		// mask to be used
		cv::Mat mask;
		if(minSaturation > 0) {
			// split the 3 channels into 3 images
			std::vector<cv::Mat> v;
			cv::split(hsv, v);
			// mask out low saturation pixels
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);
		}

		// prepare arguments for 1D hue histogram
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		channels[0] = 0; // the hue channel

		// compute the histogram
		cv::calcHist(&hsv, 1,
					channels,
					mask,
					hist,
					1,
					histSize,
					ranges);

		return hist;
	} 
};

class ContentFinder {
private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];

	float threshold;
	cv::MatND histogram;

public:
	ContentFinder(): threshold(-1.0f) {
		ranges[0] = ranges[1] = ranges[2] = hranges;
	}
	// set the threshold on hist values [0,1]
	void setThreshold(float t) {
		threshold = t;
	}
	// get threshold 
	float getThreshold() const {
		return threshold;
	}
	// set histogram
	void setHistogram(const cv::MatND &h ) {
		histogram = h;
		cv::normalize(histogram, histogram, 1.0);
	}
	// backprojection
	cv::Mat find(const cv::Mat& image,
				float minValue, float maxValue,
				int *channels, int dim) {
		cv::Mat result;

		hranges[0] = minValue; hranges[1] = maxValue;
		for(int i=0; i<dim; i++) 
			this->channels[i] = channels[i];

		cv::calcBackProject(&image, 1,	// input image
						channels,	// list of channels
						histogram,	// normalized histogram
						result,		// mat containing res
						ranges,		// range of values
						255.0
		);

		if(threshold>0.0)
			cv::threshold(result, result, 255*threshold, 255, cv::THRESH_BINARY);

		return result;
	}
};

int main(int argc, char ** argv) {
	// read ref image
	cv::Mat image = cv::imread("baboon1.jpg");
	// roi for baboon's face
	cv::Mat imageROI = image(cv::Rect(110, 260, 35, 40));
	// get the hue histogram
	int minSat = 65;
	ColorHistogram hc;
	cv::MatND colorHist = hc.getHueHistogram(imageROI, minSat);

	ContentFinder finder;
	finder.setHistogram(colorHist);

	// open new image to compare
	image = cv::imread("baboon3.jpg");
	cv::Mat hsv;
	// convert to hsv
	cv::cvtColor(image, hsv, CV_BGR2HSV);
	// split image
	std::vector<cv::Mat> v;
	cv::split(hsv, v);
	// identify pixels with low saturation
	cv::threshold(v[1], v[1], minSat, 255, cv::THRESH_BINARY);

	int ch[1];
	ch[0] = 0;
	// get the back projection of the hue histogram
	cv::Mat result = finder.find(hsv, 0.0f, 180.0f, ch, 1);
	cv::bitwise_and(result, v[1], result);

	cv::Rect rect(110, 260, 35, 40);
	cv::rectangle(image, rect, cv::Scalar(0, 0, 255));
	cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER, 10, 0.01);
	cv::meanShift(result, rect, criteria);
	cv::rectangle(image, rect, cv::Scalar(0, 255, 0));

	cv::namedWindow("Mean Shift");
	cv::imshow("Mean Shift", image);

	cv::waitKey();
	return 0;
}