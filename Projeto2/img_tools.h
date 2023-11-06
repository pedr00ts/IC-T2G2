#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Img_Tools{
    protected:
        Mat img_in;

    public:
        virtual Mat apply() {
            Mat img_out = img_in;
            return img_out;
        }
};

class Inv_Colors: public Img_Tools{};
class Mirror: public Img_Tools{};
class Rotate: public Img_Tools{};
class Brightness: public Img_Tools{};