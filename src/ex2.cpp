#include <string>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int main( int argc, char **argv ) {

  cv::namedWindow("Example3", cv::WINDOW_AUTOSIZE);

  cv::VideoCapture cap;
  auto filename = std::string(argv[1]);

  std::cout << "Opening [" << filename << "]\n";

  cap.open(filename);

  cv::Mat frame;

  std::cout << "Starting loop\n";

  while(cap.isOpened()) {
    cap >> frame;

    // std::cout << frame.elemSize() << "\n";

    if(frame.empty()) {
      std::cout << "Empty frame\n";
      break;
    }
    cv::imshow("Example3", frame);
    if((cv::waitKey(33) & 0xff) == 'q') {
      std::cout << "Key pressed\n";
      break;
    }
  }

  std::cout << "Ending loop\n";

  return 0;
}
