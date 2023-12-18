#include <opencv2/opencv.hpp>
#include "imgcodec.h"

void usage(char* argv[]) {
    cout << "Usage: " << argv[0] << " <filein> <op> <fileout> <pred_mode> [m]\n";
    cout << "op: -e -> encode; -d -> decode; pred_mode -> {0-7}; m >= 0\n";
}

int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 6) {
        cerr << "Error: wrong arg count\n";
        usage(argv);
        return 1;
    }

    uint32_t  m;
    uchar pred_mode;
    if (argc == 6){
        m = (uint32_t)stoi(argv[5]);
        if (m > MAX_USHORT_SIZE || m == 0) {
            cerr << "Error: invalid m value\n";
            usage(argv);
            return 2;
        }
    } else
        m = 32;

    pred_mode = (uchar)stoi(argv[4]);
    if (pred_mode > 7) {
        cerr << "Error: invalid pred_mode value\n";
        usage(argv);
        return 2;
    }

    imgCodec codec{m, pred_mode};

    switch(argv[2][1]) {
        case 'e':
            codec.encode(argv[1], argv[3]);
            break;           
        case 'd':
            codec.decode(argv[1], argv[3]);
            break;
        default:
            cerr << "Error: invalid op value\n";
            usage(argv);
            return 2;
    }
    
    return 0;
}