#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MorphoFeatures {
private:
	// threshold to produce binary image
	int threshold;
	// structuring elements used in corner detection
	cv::Mat cross;
	cv::Mat diamond;
	cv::Mat square;
	cv::Mat x;

	void applyThreshold(cv::Mat &result) {
		if(threshold>0) {
			cv::threshold(result, result, threshold, 255, cv::THRESH_BINARY_INV);
		}
	}

public:
	MorphoFeatures(): threshold(-1),
			cross(5,5,CV_8U,cv::Scalar(0)),
			diamond(5,5,CV_8U,cv::Scalar(1)),
			square(5,5,CV_8U,cv::Scalar(1)),
			x(5,5,CV_8U,cv::Scalar(0)) {

		// creating cross
		for(int i=0; i<5; i++) {
			cross.at<uchar>(2, i) = 1;
			cross.at<uchar>(i, 2) = 1;
		}
		// creating diamond
		diamond.at<uchar>(0,0) = 0;
		diamond.at<uchar>(0,1) = 0;
		diamond.at<uchar>(1,0) = 0;
		diamond.at<uchar>(4,4) = 0;
		diamond.at<uchar>(3,4) = 0;
		diamond.at<uchar>(4,3) = 0;
		diamond.at<uchar>(4,0) = 0;
		diamond.at<uchar>(4,1) = 0;
		diamond.at<uchar>(3,0) = 0;
		diamond.at<uchar>(0,4) = 0;
		diamond.at<uchar>(0,3) = 0;
		diamond.at<uchar>(1,4) = 0;
		// creating x shape
		for(int i=0; i<5; i++) {
			x.at<uchar>(i,i) = 1;
			x.at<uchar>(4-i,i) = 1;
		}
	}
	void setThreshold(int t) {
		threshold = t;
	}

	cv::Mat getEdges(const cv::Mat &image) {
		// get image gradient
		cv::Mat result;
		cv::morphologyEx(image, result, cv::MORPH_GRADIENT, cv::Mat());

		// apply threshold to obtain binary image
		applyThreshold(result);
		return result;
	}

	cv::Mat getCorners(const cv::Mat &image) {
		cv::Mat result;

		// dilate with cross
		cv::dilate(image, result, cross);

		// erode with diamond
		cv::erode(image, result, diamond);

		cv::Mat result2;
		// dilate with a X
		cv::dilate(image, result2, x);
		// erode with square
		cv::erode(image, result2, square);

		// corners are obtained by differencing two closed images
		cv::absdiff(result2, result, result);

		// apply threshold to obtain binary image
		applyThreshold(result);

		return result;
	}

	void drawOnImage(const cv::Mat &binary, cv::Mat &image) {
		cv::Mat_<uchar>::const_iterator it = binary.begin<uchar>();
		cv::Mat_<uchar>::const_iterator itend = binary.end<uchar>();
		for(int i=0; it!=itend; ++it,++i) {
			if(!*it) 
				cv::circle(image, cv::Point(i%image.step, i/image.step), 5, cv::Scalar(255, 0, 0));
		}
	}
};

int main(int argc, char ** argv) {
	// load image
	cv::Mat image = cv::imread("building.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	MorphoFeatures morpho;
	morpho.setThreshold(40);

	// get the edges
	cv::Mat edges;
	edges = morpho.getEdges(image);

	cv::namedWindow("Edges");
	cv::imshow("Edges", edges);

	// get corners
	cv::Mat corners;
	corners = morpho.getCorners(image);
	// display corners
	morpho.drawOnImage(corners, image);
	cv::namedWindow("Corners");
	cv::imshow("Corners", image);

	cv::waitKey();
	return 0;
}