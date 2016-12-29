#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {

  auto winName = "Example2_11";
  auto winLogPolar = "Log_Polar";

  cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
  cv::namedWindow(winLogPolar, cv::WINDOW_AUTOSIZE);

  cv::VideoCapture capture;

  // if(argc == 1) {
    capture.open(0);
  // } else {
  //  capture.open(argv[1]);
  // }
  
  if(!capture.isOpened()) {
    std::cerr << "Couldn't open capture" << std::endl;
    return -1;
  }

  double fps = capture.get(cv::CAP_PROP_FPS);
  cv::Size size(
    (int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
    (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT)
  );

  cv::VideoWriter writer;

  writer.open(argv[1], CV_FOURCC('M', 'J', 'P', 'G'), fps, size);

  cv::Mat logpolar_frame, bgr_frame;

  for(;;) {

      capture >> bgr_frame;

      if(bgr_frame.empty()) {
        break;
      }

      cv::imshow(winName, bgr_frame);

      cv::logPolar(
        bgr_frame, 
        logpolar_frame,
        cv::Point2f(
          bgr_frame.cols / 2,
          bgr_frame.rows / 2
        ),
        40,
        cv::WARP_FILL_OUTLIERS
      );

      cv::imshow(winLogPolar, logpolar_frame);

      writer << logpolar_frame;

      auto c = cv::waitKey(10);

      if(c == 27) break;
  }

  capture.release();
}