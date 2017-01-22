#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;

// global variables

// Float, 3-ch images
cv::Mat IavgF, IdiffF, IprevF, IhiF, IlowF;
cv::Mat tmp, tmp2;

// Float, 1-ch images (result of splitting 3-channel images)
vector<cv::Mat> Igray(3);
vector<cv::Mat> Ilow(3);
vector<cv::Mat> Ihi(3);

// Float, 1-ch image
cv::Mat Imaskt;

// Image counter
float Icount;

void allocateImages(const cv::Mat& I) {
  cv::Size sz = I.size();

  IavgF = cv::Mat::zeros(sz, CV_32FC3);
  IdiffF = cv::Mat::zeros(sz, CV_32FC3);
  IprevF = cv::Mat::zeros(sz, CV_32FC3);
  IhiF = cv::Mat::zeros(sz, CV_32FC3);
  IlowF = cv::Mat::zeros(sz, CV_32FC3);
  Icount = 0.00001; // protect against div by zero
  
  tmp = cv::Mat::zeros(sz, CV_32FC3);
  tmp2 = cv::Mat::zeros(sz, CV_32FC3);

  Imaskt = cv::Mat::zeros(sz, CV_32FC1);
}

void accumulateBackground(cv::Mat& I) {
  static int first = 1;
  I.convertTo(tmp, CV_32FC3);
  if(!first) {
    IavgF += tmp; // accumulate frame data for avg calc

    cv::absdiff(tmp, IprevF, tmp2);
    IdiffF += tmp2; // accumulate frame difference

    Icount += 1.0; // inc frame count
  } else {
    first = 0;
  }
  IprevF = tmp;
}

void setHighThreshold(float scale) {
  IhiF = IavgF + (IdiffF * scale);
  cv::split(IhiF, Ihi);
}

void setLowThreshold(float scale) {
  IlowF = IavgF - (IdiffF * scale);
  cv::split(IlowF, Ilow);
}

void createModelsFromStats() {
  IavgF *= (1.0 / Icount);
  IdiffF *= (1.0 / Icount);

  IdiffF += cv::Scalar(1.0, 1.0, 1.0);
  setHighThreshold(7.0);
  setLowThreshold(6.0);
}

void backgroundDiff(cv::Mat& I, cv::Mat& Imask) {
  I.convertTo(tmp, CV_32F);
  cv::split(tmp, Igray);
  
  // channel 1
  cv::inRange(Igray[0], Ilow[0], Ihi[0], Imask);

  // channel 2
  cv::inRange(Igray[1], Ilow[1], Ihi[1], Imaskt);
  Imask = cv::min(Imask, Imaskt);

  // channel 3
  cv::inRange(Igray[2], Ilow[2], Ihi[2], Imaskt);
  Imask = cv::min(Imask, Imaskt);
  
  Imask = 255 - Imask;
}

void help( char** argv ) {
  cout << "\n"
    << "Train a background model on incoming video, then run the model\nCall:\n"
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

  // build model
  int allocated = 0;
  cv::Mat image;
  while(1) {
    cap >> image;
    if(!image.data) {
      exit(0);
    }
    if(!allocated) {
      allocateImages(image);
      allocated = 1;
    }

    accumulateBackground(image);
    cv::imshow(argv[0], image);
    if((cv::waitKey(7) & 255) == 0x20) {
      break;
    }
  }

  // create model for background
  createModelsFromStats();

  // testing model
  cv::Mat mask;
  while(1) {
    cap >> image;
    if(!image.data) {
      exit(0);
    }
    backgroundDiff(image, mask);
    
    cv::split(image, Igray);
    Igray[2] = cv::max(mask, Igray[2]);
    cv::merge(Igray, image);

    cv::imshow(argv[0], image);
    if((cv::waitKey(7) & 255) == 0x20) {
      break;
    }
  }

  return 0;
}

