#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Img_Tools{
    protected:
        Mat img_in;

    public:

        virtual ~Img_Tools() {}  // Adicione um destrutor virtual

        virtual Mat apply() {
            Mat img_out = img_in;
            return img_out;
        }
};

class Inv_Colors: public Img_Tools{
    private:

    public:
        Inv_Colors (Mat img_in) {
            Inv_Colors::img_in = img_in;
        }

        virtual Mat apply() override{
            cout << "apply called\n";
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

        

};
class Mirror: public Img_Tools{
    private:
        char type;

    public:
        Mirror (Mat img_in, char type) {
            Img_Tools::img_in = img_in;
            Mirror::type = type;
        }
        virtual Mat apply() override{ 
            cout << "apply called\n";
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

        virtual Mat apply() override{
            cout << "apply called\n";
            Mat brt_img = Mat::zeros(img_in.rows, img_in.cols, CV_8UC3);
            Vec3b pixel;

            for (int i = 0; i < brt_img.rows; i++) {
                for(int j = 0; j < brt_img.cols; j++) {
                    pixel = img_in.at<Vec3b>(i,j);
                    //cout << "Pixel original: " << pixel[0] << " " << pixel[1] << " " << pixel[2] << '\n';
                    for (short c = 0; c < 3; c++){
                        pixel[c] = pixel[c] * factor;

                        if (pixel[c] > 255){
                            pixel[c] = 255;
                        }
                    }
                    brt_img.at<Vec3b>(i,j) = pixel;
                }
            }

            return brt_img;
        }
};