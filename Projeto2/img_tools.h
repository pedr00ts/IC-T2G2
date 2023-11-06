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

        Mat apply() {
            cout << "apply called\n";
            Mat inv_img = img_in.clone();
            cout << "image cloned\n";
            Vec3b pixel;

            for (int i = 0; i < inv_img.rows; i++) {
                for(int j = 0; i < inv_img.cols; j++) {
                    pixel = inv_img.at<Vec3b>(i,j);
                    cout << "Pixel original: " << pixel[0] << " " << pixel[1] << " " << pixel[2] << '\n';
                    for (short c = 0; c < 3; c++)
                        pixel[c] = 255 - pixel[c];
                    inv_img.at<Vec3b>(i,j) = pixel;
                }
            }

            return inv_img;
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