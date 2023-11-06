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

class Inv_Colors: public Img_Tools{
    private:

    public:
        Inv_Colors (Mat img_in) {
            Img_Tools::img_in = img_in;
        }

};
class Mirror: public Img_Tools{
    private:
        char type;

    public:
        Mirror (Mat img_in, char type) {
            Img_Tools::img_in = img_in;
            Mirror::type = type;
        }
};
class Rotate: public Img_Tools{
    private:
        int nRotations;

    public:
        Rotate (Mat img_in, int n) {
            Img_Tools::img_in = img_in;
            Rotate::nRotations = n;
        }
};
class Brightness: public Img_Tools{
    private:
        float factor;

    public:
        Brightness (Mat img_in, float f) {
            Img_Tools::img_in = img_in;
            Brightness::factor = f;
        }
};