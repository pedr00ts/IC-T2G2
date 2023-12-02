#include <iostream>
#include <opencv2/opencv.hpp>
#include "sndCodec.h"

using namespace std;

void usage(char *argv[]){
  cerr << "Usage: " << argv[0] << " <operation> <fileIn> <fileOut> <mode> [m]\n";
  cerr << "	List of operations:\n";
	cerr << "	-e      encode <fileIn> to <fileOut>.\n";
	cerr << "	-d      decode <fileIn> to <fileOut>.\n";

  cerr << "	List of params:\n";
	cerr << "	mode:   (0) -> sign and magnitude (1) -> positive/negative interleaving.\n";
	cerr << "	m:      optional golomb modulus parameter.\n";
}

enum operation{
    e = 'e', d = 'd', c = 'c'
};

int main(int argc, char *argv[])
{
    // check number of args
    if(argc < 5 || argc > 6) {
            cerr << "Error: wrong number of args\n";
            usage(argv);
            return 1;
        }
    
    sndCodec codec;
    bool mode = stoi(argv[4]);
    if (argc == 6){
      codec = {stoi(argv[5]), mode};
    } else {
      codec = {mode};
    }
    
    
    switch(argv[1][1]) {
        case e: {          
          SndfileHandle sndFile = {argv[2]};
          codec.encode(sndFile, argv[3]);
          break;
        }
        case d: {
          codec.decode(argv[2], argv[3]);
          break;
        }
        case c: {
          Golomb golomb(stoi(argv[5]), stoi(argv[4]));
          GolombStream gstream(golomb, argv[3]);
          int number(stoi(argv[2]));
          cout << "\nEncoding number " << argv[2] << " to " << argv[3] << " with m=" << argv[5] << " and mode=" << argv[4] << '\n';
          gstream.encodeNext(number);
          break;
        }
        default:
            cerr << "Error - invalid operation." << '\n';
            usage(argv);
            return 1; 
    }
    

    return 0;
}
