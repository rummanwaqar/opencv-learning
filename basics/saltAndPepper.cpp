/* Replaces random pixels with white. Takes source image as first argment */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

void salt(cv::Mat &img, int n) {
  // passing the image by reference so it can be modified
  for(int k=0; k<n; k++) {
    // select random pixels
    int i=rand()%img.cols;
    int j=rand()%img.rows;

    if(img.channels() == 1) { // gray level image
      img.at<uchar>(j,i) = 255;
    } else {                // color image
      img.at<cv::Vec3b>(j,i)[0] = 255;
      img.at<cv::Vec3b>(j,i)[1] = 255;
      img.at<cv::Vec3b>(j,i)[2] = 255;
    }
  }
}

int main(int argc, char ** argv) {
  cv::Mat img = cv::imread(argv[1]);

  if(!img.data) return -1;

  salt(img, 3000);
  cv::namedWindow("Salted Image");
  cv::imshow("Salted Image", img);
  cv::waitKey(0);
  return 0;
}
