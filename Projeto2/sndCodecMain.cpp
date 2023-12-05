#include <iostream>
#include "sndCodec.h"

using namespace std;

void usage(char *argv[]){
  cerr << "Usage: " << argv[0] << " <operation> <fileIn> <fileOut> <mode> [m]" << endl;
  cerr << "	List of operations:" << endl;
	cerr << "	-e      encode <fileIn> to <fileOut>." << endl;
	cerr << "	-d      decode <fileIn> to <fileOut>." << endl;

  cerr << "	List of params:" << endl;
	cerr << "	mode:   (0) -> sign and magnitude (1) -> positive/negative interleaving." << endl;
	cerr << "	m:      optional golomb modulus parameter." << endl;
}

enum operation{
    encode = 'e', decode = 'd'
};

int main(int argc, char *argv[])
{
  // check number of args
  if(argc < 5 || argc > 6) {
    cerr << "Error: wrong number of args\n";
    usage(argv);
    return 1;
  }
  
  // get files paths
  string finName, foutName;
  finName = argv[2];
  foutName = argv[3];

  // get encoding/decoding parameters
  sndCodec codec;
  bool mode = stoi(argv[4]);
  if (argc == 6){
    codec = {stoi(argv[5]), mode};
  } else {
    codec = {mode};
  }

  // execute desired operation
  filesystem::path fileIn(finName), fileOut(foutName);
  switch(argv[1][1]) {
      case encode: {          
        SndfileHandle sndFile = {finName};  // file to encode
        codec.encode(sndFile, foutName);
        break;
      }
      case decode: {
        BitStream encFile = {finName};      // file to decode
        codec.decode(encFile, foutName);
        break;
      }
      default:
          cerr << "Error - invalid operation." << endl;
          usage(argv);
          return 1; 
  }

  // show encoding/decoding stats
  cout << " in: " << filesystem::file_size(fileIn)  << " bytes" << endl;
  cout << "out: " << filesystem::file_size(fileOut) << " bytes" << endl;

  return 0;
}
