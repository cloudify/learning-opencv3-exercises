#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {

  cv::Mat image = cv::imread(argv[1], -1);
  if(image.empty()) {
    return -1;
  }

  auto windowInName = "Example2_5-in";
  auto windowOutName = "Example2_5-out";

  // windows to show input and output images
  cv::namedWindow(windowInName, cv::WINDOW_AUTOSIZE);
  cv::namedWindow(windowOutName, cv::WINDOW_AUTOSIZE);

  // show image
  cv::imshow(windowInName, image);

  // image buffer to hold smoothed image
  cv::Mat out;

  // Do the smoothing
  cv::GaussianBlur(image, out, cv::Size(5, 5), 3, 3);
  cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);

  // Show smoothed image in the output window
  cv::imshow(windowOutName, out);

  // Wait for the user to hit a key
  cv::waitKey(0);
}
