#include <iostream>
#include <opencv2/opencv.hpp>
#include "img_tools.h"

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

  Img_Tools tool;
  Mat new_img;
  switch(argv[2][1]) {
    case n: 
        {
          tool = Inv_Colors {img};         
          break;
        }   
    case m:  
        {   
          // check parameters 
          char direction = (char) tolower(argv[3][0]);
          if(direction != 'h' && direction != 'v'){
              cerr << "Error: invalid direction: " << direction  << "\n";
              usage(argv);
              return 1;
          }              

          tool = Mirror {img, direction};                
          
          break;      
        }      
    case r: 
        {
          // check parameters 
          int times = stoi(argv[3]);

          tool = Rotate {img, times};
          break;
        }  
    case l: 
        {
          // check parameters
          float intensity = stof(argv[3]);

          if (intensity < 0) {
            cerr << "Error: invalid intensity! Should be a positive value. " << "\n";
            usage(argv);
            return 1;
          }

          tool = Brightness {img, intensity}; 
          break;
        }  
  }
  
  // apply tool operation
  new_img = tool.apply();
  // write result image
  cv::imwrite(argv[-1], new_img);
  return 0;
}