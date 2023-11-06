#include <iostream>
#include <opencv2/opencv.hpp>
#include "ppm_extract.h"

using namespace std;
using namespace cv;


void usage(char *argv[]){
    cerr << "Usage: " << argv[0] << " <input file> <channel> <output file> \n";
    cerr << "	List of available channels:\n";
	cerr << "	-r            for channel RED\n";
	cerr << "	-g            for channel GREEN\n";
	cerr << "	-b            for channel BLUE\n";
}

enum CHANNEL{
    r = 'r', g = 'g', b = 'b'
};

int main(int argc, char *argv[])
{
    if(argc != 4) {
		cerr << "Error: wrong number of args\n";
		usage(argv);
		return 1;
	}
    
    // std::string image_path = "../imagesamples/lena.ppm";
    // std::string mew_image_path = "../imagesamples/lenablue.ppm";
    int channel;
    switch(argv[2][1]){
        case r: 
            {
                channel = 2;
                break;
            }   
        case g:  
            {
                channel = 1;  
                break;      
            }      
        case b: 
            {
                channel = 0;
                break;
            }       
    
    }
    Mat img = imread(argv[1], IMREAD_COLOR);


    PPMExtract original = PPMExtract{channel,img};

    Mat new_img = original.extract();


    imwrite(argv[3],new_img);
    return 0;
}