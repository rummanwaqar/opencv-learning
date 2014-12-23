#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
	cv::Mat image = cv::imread("group.jpg");

	// define bounding rect, pixels outside will be labeled bg
	cv::Rect rectangle(10, 100, 380, 180);

	cv::Mat result;
	cv::Mat bgModel, fgModel;
	cv::grabCut(image, 		// image
				result,		// segmentation result
				rectangle,	// rectangle containing fg
				bgModel, fgModel,	// models
				5,			// no of iterations
				cv::GC_INIT_WITH_RECT	// use rectangle
	);

	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(foreground, result);

	result = result & 1;


	cv::imshow("Result", foreground);
	cv::waitKey();
	return 0;
}