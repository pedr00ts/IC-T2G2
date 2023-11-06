#include <iostream>
#include <opencv2/opencv.hpp>
//#include "img_tools.h"

using namespace std;
using namespace cv;

void usage(char *argv[]){
  cerr << "Usage: " << argv[0] << " <input file> <tool> [params] <output file> \n";
  cerr << "	List of available tools:\n";
	cerr << "	-n             negative tool\n";
	cerr << "	-m [direction] mirror tool\n";
	cerr << "	-r [times]     90º rotation tool\n";
  cerr << "	-l [intensity] luminosity tool\n";

  cerr << "	List of available params:\n";
	cerr << "	direction: <char> mirroring direction ([H]orizontal | [V]ertical).\n";
	cerr << "	times: <int> number of 90º rotations.\n";
  cerr << "	intensity: <float> light intensity factor. \n";
}

enum tool{
    n = 'n', m = 'm', r = 'r', l = 'l' 
};

int main(int argc, char *argv[])
{
  // check number of args
  if(argc < 4 || argc > 5) {
		cerr << "Error: wrong number of args\n";
		usage(argv);
		return 1;
	}

  // check input file
  Mat img = imread(argv[1], IMREAD_COLOR);

  Mat new_img {};
  switch(argv[2][1]){
        case n: 
            {
                              
                break;
            }   
        case m:  
            {

                break;      
            }      
        case r: 
            {
                
                break;
            }  
        case l: 
            {
                
                break;
            }  
  
  }
    
  // write result image
  imwrite(argv[3], new_img);
  return 0;
}