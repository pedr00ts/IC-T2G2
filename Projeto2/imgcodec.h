#include <opencv2/opencv.hpp>
#include "golomb.h"
#include "bitstream.h"
#define MAX_USHORT_SIZE 65536

using namespace std;
using namespace cv;

// Classe imgCodec -> codificação para imagens grayscale em modo JPEG
#ifndef IMGCODEC
#define IMGCODEC
class imgCodec {
    protected:
        Golomb golomb;
        uchar pred_mode;    // default prediction mode
        uint predicted_value(uint a, uint b, uint c, uchar mode);
    

    public:
        imgCodec(uint32_t m, uchar pred_mode);     // constructor
        imgCodec() = default;
        ~imgCodec() = default;
        void encode(string img_path, string code_path);       // encodes image at img_path into the file at code_path 
        void decode(string code_path, string img_path);       // decodes file at code_path into the file at img_path 
};  
#endif