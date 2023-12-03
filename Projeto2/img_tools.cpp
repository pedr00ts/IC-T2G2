#include "img_tools.h"


 Img_Tools::~Img_Tools(){};

Mat Img_Tools::apply() {
    cout << "apply called - Img_tools\n";
    Mat img_out = img_in;
    return img_out;
}



Inv_Colors::Inv_Colors (Mat img_in) {
    Inv_Colors::img_in = img_in;
}

Mat Inv_Colors::apply(){
    cout << "apply called - Inv_colors\n";
    Mat inv_img = Mat::zeros(img_in.rows, img_in.cols, CV_8UC3);
    Vec3b pixel;

    for (int i = 0; i < inv_img.rows; i++) {
        for(int j = 0; j < inv_img.cols; j++) {
            pixel = img_in.at<Vec3b>(i,j);
            //cout << "Pixel original: " << pixel[0] << " " << pixel[1] << " " << pixel[2] << '\n';
            for (short c = 0; c < 3; c++)
                pixel[c] = 255 - pixel[c];
            inv_img.at<Vec3b>(i,j) = pixel;
        }
    }

    return inv_img;
}



Mirror::Mirror (Mat img_in, char type) {
  Img_Tools::img_in = img_in;
  Mirror::type = type;
}
Mat Mirror::apply() { 
  cout << "apply called - Mirror\n";
  Mat mir_img = Mat::zeros(img_in.rows, img_in.cols, CV_8UC3);

  for (int i = 0; i < mir_img.rows; i++) {
      for (int j = 0; j < mir_img.cols; j++) {
          if (type == 'v') {
              mir_img.at<Vec3b>(mir_img.rows - 1 - i, j) = img_in.at<Vec3b>(i, j);
          } else if (type == 'h') {
              mir_img.at<Vec3b>(i, mir_img.cols - 1 - j) = img_in.at<Vec3b>(i, j);
          }
      }
  }
  return mir_img;

}


Rotate::Rotate (Mat img_in, int n) {
    Img_Tools::img_in = img_in;
    Rotate::nRotations = n;
}

Mat Rotate::apply(){
    cout << "apply called - Rotate\n";
    Mat rot_img;

    switch (nRotations % 4) {
        case 0:
            rot_img = img_in.clone();
            break;
        case 1:
            rot_img = cv::Mat::zeros(img_in.cols, img_in.rows, CV_8UC3);
            for (int i = 0; i < img_in.rows; i++) {
                for (int j = 0; j < img_in.cols; j++) {
                    rot_img.at<cv::Vec3b>(j, img_in.rows - 1 - i) = img_in.at<cv::Vec3b>(i, j);
                }
            }
            break;
        case 2:
            rot_img = img_in.clone();
            for (int i = 0; i < img_in.rows; i++) {
                for (int j = 0; j < img_in.cols; j++) {
                    rot_img.at<cv::Vec3b>(img_in.rows - 1 - i, img_in.cols - 1 - j) = img_in.at<cv::Vec3b>(i, j);
                }
            }
            break;
        case 3:
            rot_img = cv::Mat::zeros(img_in.cols, img_in.rows, CV_8UC3);
            for (int i = 0; i < img_in.rows; i++) {
                for (int j = 0; j < img_in.cols; j++) {
                    rot_img.at<cv::Vec3b>(img_in.cols - 1 - j, i) = img_in.at<cv::Vec3b>(i, j);
                }
            }
            break;
    }

    return rot_img;
}



Brightness::Brightness (Mat img_in, float f) {
    Img_Tools::img_in = img_in;
    Brightness::factor = f;
}

Mat Brightness::apply(){
    cout << "apply called - Brightness\n";
    Mat brt_img = Mat::zeros(img_in.rows, img_in.cols, CV_8UC3);
    Vec3b pixel;

    for (int i = 0; i < brt_img.rows; i++) {
        for(int j = 0; j < brt_img.cols; j++) {
            pixel = img_in.at<Vec3b>(i,j);
            //cout << "Pixel original: " << pixel[0] << " " << pixel[1] << " " << pixel[2] << '\n';
            for (short c = 0; c < 3; c++){
                short aux = pixel[c] + factor;
                if (aux > 255){
                    aux = 255;
                }else if(aux < 0){
                    aux = 0;
                }

                pixel[c] = aux;

            }
            brt_img.at<Vec3b>(i,j) = pixel;
        }
    }

    return brt_img;
}