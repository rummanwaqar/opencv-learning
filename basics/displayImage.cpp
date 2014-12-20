/*Flips an image and saves it. 
  first argument image file to read, second argment file name to save
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char ** argv) {
  Mat image = imread(argv[1]);
  cout << "size: " << image.size().height << ", " << image.size().width << endl;
  if(!image.data) return -1;
  namedWindow("Original Image");
  imshow("Original Image", image);
  Mat result;
  flip(image, result, 1);
  namedWindow("Flipped Image");
  imshow("Flipped Image", result);
  waitKey(0);
  imwrite(argv[2], result);
  
  return 0;
}
