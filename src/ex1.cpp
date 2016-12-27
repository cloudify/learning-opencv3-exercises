#include <opencv2/opencv.hpp>

int main( int argc, char** argv ) {
  // read image passed in cmd line
  cv::Mat img = cv::imread(argv[1],-1);
  // check whether img is empty
  if( img.empty() ) return -1;

  // init window Example1
  cv::namedWindow( "Example1", cv::WINDOW_AUTOSIZE );

  // show image
  cv::imshow( "Example1", img );

  // wait for a key
  cv::waitKey( 0 );

  // destroy window Example1
  cv::destroyWindow( "Example1" );
  
  return 0;
}
