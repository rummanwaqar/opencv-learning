#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class WatershedSegmenter {
private:
	cv::Mat markers;

public:
	void setMarkers(const cv::Mat & markerImage) {
		// convert to image of ints
		markerImage.convertTo(markers, CV_32S);
	}

	cv::Mat process(const cv::Mat &image) {
		// apply watershed
		cv::watershed(image, markers);

		return markers;
	}

	cv::Mat getSegmentation() {
		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U);
		return tmp;
	}

	cv::Mat getWatersheds() {
		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U, 255, 255);
		return tmp;
	}
};	

int main() {
	cv::Mat image = cv::imread("group.jpg");

	cv::Mat binary = cv::imread("binary.bmp", 0);

	cv::namedWindow("Inital Image");
	cv::imshow("Inital Image", image);

	// eliminate noise and smaller objects
	cv::Mat fg;
	cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 6);

	// identify background
	cv::Mat bg; 
	cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 6);
	cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

	// create marker image
	cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
	markers = fg + bg;

	cv::namedWindow("Markers");
	cv::imshow("Markers", markers);

	// segmentation
	WatershedSegmenter segmenter;
	segmenter.setMarkers(markers);
	segmenter.process(image);

	cv::namedWindow("Segmentation");
	cv::imshow("Segmentation", segmenter.getSegmentation());

	cv::namedWindow("Watersheds");
	cv::imshow("Watersheds", segmenter.getWatersheds());

	cv::waitKey();
	return 0;
}