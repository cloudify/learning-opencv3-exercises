#include <opencv2/opencv.hpp>

void my_mouse_callback(
  int event, int x, int y, int flags, void *param
);

cv::Rect box;
bool drawing_box = false;

void draw_box(cv::Mat& img, cv::Rect box) {
  cv::rectangle(
    img,
    box.tl(),
    box.br(),
    cv::Scalar(0x00, 0x00, 0xff)
  );
}

int main(int argc, char** argv) {

  box = cv::Rect(-1, -1, 0, 0);
  cv::Mat image(200, 200, CV_8UC3), temp;
  image.copyTo(temp);

  box = cv::Rect(-1, -1, 0, 0);
  image = cv::Scalar::all(0);

  const auto winName = "Box Example";
  cv::namedWindow(winName);

  cv::setMouseCallback(
    winName,
    my_mouse_callback,
    (void*)&image
  );

  for(;;) {
    image.copyTo(temp);
    if(drawing_box) {
      draw_box(temp, box);
    }
    cv::imshow(winName, temp);
    if(cv::waitKey(15) == 27) {
      std::cout << "Key pressed!" << std::endl;
      break;
    }

  }

  return(0);
}

void my_mouse_callback(
  int event, int x, int y, int flags, void *param
) {
  cv::Mat& image = *(cv::Mat*) param;

  switch(event) {

    case cv::EVENT_MOUSEMOVE: {
      if(drawing_box) {
        box.width = x - box.x;
        box.height = y - box.y;
      }
    }
    break;

    case cv::EVENT_LBUTTONDOWN: {
      drawing_box = true;
      box = cv::Rect(x, y, 0, 0);
    }
    break;

    case cv::EVENT_LBUTTONUP: {
      drawing_box = false;
      if(box.width < 0) {
        box.x += box.width;
        box.width *= -1;
      }
      if(box.height < 0) {
        box.y += box.height;
        box.height *= -1;
      }
      draw_box(image, box);
    }
    break;
  }
}