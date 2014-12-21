/* Makes the image sharper 
   first argument is the image to be processed
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void sharpen(const cv::Mat &image, cv::Mat &result) {
  // allocate 
  result.create(image.size(), image.type());

  // loop over all rows except first & last
  for(int j=1; j<image.rows-1; j++) {
    const uchar* previous = image.ptr<const uchar>(j-1); // prev row
    const uchar* current = image.ptr<const uchar>(j); // current row
    const uchar* next = image.ptr<const uchar>(j+1);  // next row
    
    uchar* output = result.ptr<uchar>(j);             // output row
    
    // loop over all cols except first & last
    for(int i=1; i<image.cols-1; i++) {
      *output++ = cv::saturate_cast<uchar>(5 * current[i] - current[i-1] - current[i+1] - previous[i] - next[i]);
    }
  }

  // set the unprocessed pixels to 0
  result.row(0).setTo(cv::Scalar(0));
  result.row(result.rows-1).setTo(cv::Scalar(0));
  result.col(0).setTo(cv::Scalar(0));
  result.col(result.cols-1).setTo(cv::Scalar(0));
}

int main(int argc, char ** argv) {
  cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat result;
  sharpen(image, result);
  cv::namedWindow("Sharpen Image");
  cv::imshow("Sharpen Image", result);
  cv::waitKey(0);
  return 0;
}
