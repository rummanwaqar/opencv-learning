/* Color reduction algorithm using iterators 
   First argument is the image to be reduced
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void colorReduce(cv::Mat &img, int n=64) {
  // obtain iterator initialized at start pos
  cv::MatIterator_<cv::Vec3b> it = img.begin<cv::Vec3b>();
  // obtain iterator end position
  cv::MatIterator_<cv::Vec3b> itend = img.end<cv::Vec3b>();

  // loop over pixels
  for(; it!=itend; ++it) {
    // process each channel of each pixel by dereferencing the iterator
    (*it)[0] = (*it)[0]/n*n + n/2;
    (*it)[1] = (*it)[1]/n*n + n/2;
    (*it)[2] = (*it)[2]/n*n + n/2;
  }
}

int main(int argc, char ** argv) {
  cv::Mat img = cv::imread(argv[1]);
  colorReduce(img);
  cv::namedWindow("Color Reduction");
  cv::imshow("Color Reduction", img);
  cv::waitKey(0);
  return 0;
}
