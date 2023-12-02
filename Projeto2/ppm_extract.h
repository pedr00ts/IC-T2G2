#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PPMExtract{

    private:
        int channel;
        Mat img_in;

    public:

        PPMExtract(int channel,Mat img);
        Mat extract();

};