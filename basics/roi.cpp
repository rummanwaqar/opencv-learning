/* adds two images of different sizes */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char ** argv) {
  cv::Mat logo = cv::imread("logo.png");
  cv::Mat image = cv::imread("boldt.jpg");

  // define image ROI
  cv::Mat imageROI = image(cv::Rect(385, 270, logo.cols, logo.rows));
  
  // load the mask
  cv::Mat mask = cv::imread("logo.png", 0);

  logo.copyTo(imageROI, mask);

  // display
  cv::namedWindow("Region of Interest");
  cv::imshow("Region of Interest", image);
  cv::waitKey(0);
  return 0;
}
