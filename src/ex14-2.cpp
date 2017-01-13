#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

struct AreaCmp {
  AreaCmp(const vector<float>& _areas): areas(&_areas) {}
  
  bool operator()(int a, int b) const {
    return (*areas)[a] > (*areas)[b];
  }
  
  const vector<float>* areas;
};

int main(int argc, char** argv) {
  cv::Mat img, img_edge, img_color;

  if(argc != 2 || ( img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE)).empty()) {
    cout << "Drawing contours\nUsage: " <<argv[0]
      <<"fruits.jpg" << endl;
    return -1;
  }

  cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
  cv::imshow("Image after threshold", img_edge);

  vector< vector< cv::Point >> contours;
  vector< cv::Vec4i > hierarchy;

  cv::findContours(
    img_edge,
    contours,
    hierarchy,
    cv::RETR_LIST,
    cv::CHAIN_APPROX_SIMPLE
  );

  cout << "\n\nHit any key to draw the next contour, ESC to quit\n\n";
  cout << "Total Contours Detected: " << contours.size() << endl;

  vector<int> sortIdx(contours.size());
  vector<float> areas(contours.size());

  for(int n = 0; n < (int)contours.size(); n++) {
    sortIdx[n] = n;
    areas[n] = cv::contourArea(contours[n], false);
  }

  // sort contours by size descending
  std::sort(sortIdx.begin(), sortIdx.end(), AreaCmp(areas));

  for(int n = 0; n < (int)sortIdx.size(); n++) {
    int idx = sortIdx[n];
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);
    cv::drawContours(
      img_color,
      contours,
      idx,
      cv::Scalar(0, 0, 255), 2, 8, hierarchy, 5 // TODO try different values
    );
    cout << "Contour #" << idx << ": area=" << areas[idx] <<
      ", nvertices=" << contours[idx].size() << endl;
    cv::imshow(argv[0], img_color);
    int k = cv::waitKey() & 255;
    
    if(k == 27) {
      break;
    }
  }
  cout << "Finished all contours\n";

  return 0;
}

