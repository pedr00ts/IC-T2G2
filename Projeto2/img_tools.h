#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class img_tools{
    protected:
        Mat img_in;

    public:
        virtual Mat apply(Mat img_out) {return img_out;}

};