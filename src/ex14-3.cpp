#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  cv::Mat img, img_edge, labels, img_color, stats, centroids;

  if(argc != 2 || ( img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE)).empty()) {
    cout << "Drawing contours\nUsage: " <<argv[0]
      <<"fruits.jpg" << endl;
    return -1;
  }

  
  cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
  cv::imshow("Image after threshold", img_edge);

  int i, nccomps = cv::connectedComponentsWithStats(
    img_edge, labels,
    stats, centroids // NOTE: cv::noArray() core dumps
  );
  cout << "Total connected components detected: " << nccomps << endl;
 
  vector <cv::Vec3b> colors(nccomps + 1);
  colors[0] = cv::Vec3b(0, 0, 0);
  for(i = 1; i <= nccomps; i++) {
    if(stats.at<int>(i-1, cv::CC_STAT_AREA) < 100) {
      // small areas are painted black
      colors[i] = cv::Vec3b(0, 0, 0);  
    } else {
      colors[i] = cv::Vec3b(rand()%256, rand()%256, rand()%256);
    }
  }
  
  img_color = cv::Mat::zeros(img.size(), CV_8UC3);
  for(int y = 0; y < img_color.rows; y++) {
    for(int x = 0; x < img_color.cols; x++) {
      int label = labels.at<int>(y, x);
      CV_Assert(0 <= label && label <= nccomps);
      img_color.at<cv::Vec3b>(y, x) = colors[label];
    }
  }

  cv::imshow("Labeled map", img_color);
  cv::waitKey();
  

  return 0;
}

