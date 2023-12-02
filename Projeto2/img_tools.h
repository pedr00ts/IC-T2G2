#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Img_Tools{
    protected:
        Mat img_in;

    public:

        virtual ~Img_Tools();  // Adicione um destrutor virtual

        virtual Mat apply();

};

class Inv_Colors: public Img_Tools{
    private:

    public:
        Inv_Colors (Mat img_in);

        virtual Mat apply() override;

        

};
class Mirror: public Img_Tools{
    private:
        char type;

    public:
        Mirror (Mat img_in, char type); 
        virtual Mat apply();
};

class Rotate: public Img_Tools{
    private:
        int nRotations;

    public:
        Rotate (Mat img_in, int n);

        virtual Mat apply() override ;
};

class Brightness: public Img_Tools{
    private:
        float factor;

    public:
        Brightness (Mat img_in, float f);

        virtual Mat apply() override;
};