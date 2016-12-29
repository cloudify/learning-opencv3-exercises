#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  cv::Mat img1, img2;

  auto example1WindowName = "Example1";
  auto example2WindowName = "Example2";

  cv::namedWindow(example1WindowName, cv::WINDOW_AUTOSIZE);
  cv::namedWindow(example2WindowName, cv::WINDOW_AUTOSIZE);

  img1 = cv::imread(argv[1]);
  cv::imshow(example1WindowName, img1);

  cv::pyrDown(img1, img2);
  cv::imshow(example2WindowName, img2);

  cv::waitKey(0);
}
