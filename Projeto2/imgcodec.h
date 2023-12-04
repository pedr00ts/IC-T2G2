#include <opencv.hpp>
#include "golomb.h"
#include "bitstream.h"

using namespace std;

// Classe imgCodec -> codificação para imagens grayscale em modo JPEG
#ifndef IMGCODEC
#define IMGCODEC
class imgCodec {
    private:
        Golomb golomb;

    public:
        imgCodec(uint32_t m, bool mode, uchar pred_mode);
        imgCodec = default();
};
#endif