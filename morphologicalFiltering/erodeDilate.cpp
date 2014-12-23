/* Erode and dilate a binary image */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char ** argv) {
	// read input image
	cv::Mat image = cv::imread("binary.bmp");

	// erode the image
	cv::Mat eroded;
	cv::erode(image, eroded, cv::Mat());

	// display eroded image
	cv::namedWindow("Eroded Image");
	cv::imshow("Eroded Image", eroded);

	// dilate image
	cv::Mat dilated;
	cv::dilate(image, dilated, cv::Mat());

	// display dilated image
	cv::namedWindow("Dilated Image");
	cv::imshow("Dilated Image", dilated);

	cv::waitKey();
	return 0;
}