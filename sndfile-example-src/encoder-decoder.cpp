#include "bitstream.h"

using namespace std;

void usage(char* argv) {
    cerr << "Usage: " << argv[0] << " <inputFile> <-d|-e> <outputFile>\n";
}

int main (int argc, char* argv) {

    if(argc != 4) {
        cerr << "Error: invalid number of arguments\n";
        usage(argv);
    }

    if (argv[2][1] == 'd') {
        // ... decode
    } else if (argv[2][1] == 'e') {
        // ... encode
    } 

    fstream fileIn {argv[1]};
    fstream fileOut {argv[3]};

    return 0;
}