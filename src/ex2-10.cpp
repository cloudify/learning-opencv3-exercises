#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {

  auto winName = "Example2_10";

  cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

  cv::VideoCapture cap;

  if(argc == 1) {
    cap.open(0);
  } else {
    cap.open(argv[1]);
  }

  if(!cap.isOpened()) {
    std::cerr << "Couldn't open capture" << std::endl;
    return -1;
  }



}