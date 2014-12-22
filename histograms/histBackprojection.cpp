/* finds content using histogram backprojection */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


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
	ColorHistogram hc;

	cv::Mat image = cv::imread("waves.jpg");
	image = hc.colorReduce(image, 32);

	// roi blue sky
	cv::Mat imageROI = image(cv::Rect(0,0,165,75));

	cv::MatND hist = hc.getHistogram(imageROI);

	ContentFinder finder;
	finder.setThreshold(0.05f);
	finder.setHistogram(hist);

	int channel[3];
	channel[0] = 0; channel[1] = 1; channel[2] = 2;

	cv::Mat result = finder.find(image, 0.0, 255.0, channel, 3);

	cv::namedWindow("Back Projection");
	cv::imshow("Back Projection", result);

	cv::waitKey();
	return 0;
}