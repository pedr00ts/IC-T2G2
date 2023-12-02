#include "ppm_extract.h"


PPMExtract::PPMExtract(int channel,Mat img){
    PPMExtract::channel = channel;
    PPMExtract::img_in = img;
}

Mat PPMExtract::extract(){
    Mat new_image = Mat::zeros(img_in.rows, img_in.cols, CV_8UC1);
    
    for (int i = 0; i < img_in.rows; i++) {
        for (int j = 0; j < img_in.cols; j++) {
            new_image.at<uchar>(i,j) = img_in.at<Vec3b>(i,j)[channel];        
        }
    }
    return new_image;   
}

