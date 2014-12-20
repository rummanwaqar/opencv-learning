/* reduce color of image by a factor of N
   first argument is the image file
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void colorReduce(const cv::Mat &img, cv::Mat &result, int n = 64) {
  // no of lines
  int nl = img.rows;
  // elements per line
  int nc = img.cols * img.channels();

  // result.create(img.rows, img.col, img.type());

  for(int j=0; j<nl; j++) {
    const uchar* data_in = img.ptr<uchar>(j);
    uchar* data_out = result.ptr<uchar>(j);
    for(int i=0; i<nc; i++) {
      data_out[i] = data_in[i]/n*n + n/2;
    }
  }
}

int main(int argc, char ** argv) {
  cv::Mat img = cv::imread(argv[1]);
  colorReduce(img, img);
  cv::namedWindow("Color Reduction");
  cv::imshow("Color Reduction", img);
  cv::waitKey(0);
}
