#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PPMExtract{

    private:
        int channel;
        Mat img_in;

    public:

        PPMExtract(int channel,Mat img){
            PPMExtract::channel = channel;
            PPMExtract::img_in = img;
        }

        Mat extract(){
            int channels[] = {0,0,0};
            Mat new_image = img_in.clone();
            
            for (int i = 0; i < img_in.rows; i++) {
                for (int j = 0; j < img_in.cols; j++) {
                    // Acesse os valores de cada canal de cor
                    channels[channel] = img_in.at<Vec3b>(i, j)[channel];
                    new_image.at<Vec3b>(i,j) = Vec3b(channels[0],channels[1],channels[2]);
                    //cout << channels[0] <<" "<< channels[1]<< " "<< channels[2] << "\n";          
                }
            }
            return new_image;   
        }

};