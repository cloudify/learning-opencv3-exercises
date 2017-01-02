#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char **argv) {

  if (argc != 2) {
    cout << "Computer Color Histogram\nUsage: " << argv[0] << " <imagename>"
         << endl;
    return -1;
  }

  // read image
  auto src = cv::imread(argv[1], 1);
  if (src.empty()) {
    cout << "Cannot load " << argv[1] << endl;
    return -1;
  }

  // convert image from BGR to HSV
  cv::Mat hsv;
  cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

  float h_ranges[] = {0, 180};
  float s_ranges[] = {0, 256};
  const float *ranges[] = {h_ranges, s_ranges};

  int histSize[] = {30, 32};
  int ch[] = {0, 1};

  cv::Mat hist;

  // compute the histogram
  cv::calcHist(&hsv, 1, ch, cv::noArray(), hist, 2, histSize, ranges, true);
  // normalize values to 0-255 (min = 0, max = 255)
  cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

  // create an image of histogram size * 10
  int scale = 10;
  cv::Mat hist_img(histSize[0] * scale, histSize[1] * scale, CV_8UC3);

  for (int h = 0; h < histSize[0]; h++) {
    for (int s = 0; s < histSize[1]; s++) {
      // get value of histogram at [h,s]
      auto hval = hist.at<float>(h, s);
      // create a grey rectangle at [h,s]
      cv::rectangle(hist_img, cv::Rect(h * scale, s * scale, scale, scale),
                    cv::Scalar::all(hval), -1);
    }
  }

  // show the image and the histogram
  cv::imshow("image", src);
  cv::imshow("H-S histogram", hist_img);

  // wait for a key
  cv::waitKey();

  return 0;
}