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
        default:
            cerr << "Error: Invalid channel\n";
		    usage(argv);
		    return 1;
    
    }
    Mat img = imread(argv[1], IMREAD_COLOR);

    if(img.empty()){
        cerr << "Error: invalid input file\n";
		return 1;
    }

    PPMExtract original = PPMExtract{channel,img};

    Mat new_img = original.extract();


    bool success = imwrite(argv[3],new_img);

    if(!success){
        cerr << "Error: Invalid output file\n";
        usage(argv);
        return 1;
    }
    return 0;
}