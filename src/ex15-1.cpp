#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;

void help( char** argv ) {
  cout << "\n"
    << "Read out RGB pixel values and store them to disk\nCall:\n"
    << argv[0] <<" avi_file\n"
    << "\n This will store to files blines.csv, glines.csv and rlines.csv\n\n"
    << endl;
}

int main(int argc, char** argv) {
  if(argc != 2) {
    help(argv);
    return -1;
  }

  cv::namedWindow(argv[0], CV_WINDOW_AUTOSIZE);

  cv::VideoCapture cap;
  if(argc < 2 || !cap.open(argv[1])) {
    cerr << "Couldn't open video file" << endl;
    help(argv);
    cap.open(0);
    return -1;
  }

  cv::Point pt1(10, 10), pt2(30, 30);
  int max_buffer;
  cv::Mat rawImage;
  ofstream b, g, r;

  b.open("blines.csv");
  g.open("glines.csv");
  r.open("rlines.csv");

  for(;;) {
    cap >> rawImage;
    if(!rawImage.data) {
      break;
    }

    // scan line
    cv::LineIterator it(rawImage, pt1, pt2);
    for(int j=0; j < it.count; j++) {
      b << (int)(*it)[0] << ",";
      g << (int)(*it)[0] << ",";
      r << (int)(*it)[0] << ",";
      (*it)[2] = 255; // mark this pixel in red
    }
    cv::imshow(argv[0], rawImage);
    int c = cv::waitKey(10);
    b << "\n";
    g << "\n";
    r << "\n";
  }

  b << endl;
  g << endl;
  r << endl;

  b.close();
  g.close();
  r.close();

  return 0;
}

