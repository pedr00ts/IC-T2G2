#include "imgcodec.h"
#include <opencv2/opencv.hpp>

imgCodec::imgCodec(uint32_t m, bool mode, uchar pred_mode) {            // usar modo unsigned
    golomb = Golomb(m, mode);
    imgCodec::pred_mode = pred_mode;
}

uint_fast8_t imgCodec::predicted_value(uint_fast8_t a, uint_fast8_t b, uint_fast8_t c, uchar mode) {
    switch(mode) {
        case 0:                 // mode 0 -> predicted value is always 0
            return 0;         
        case 1:
            return a;
        case 2:
            return b;
        case 3:
            return c;
        case 4:
            return a+b-c;
        case 5:
            return a+(b-c)/2;
        case 6:
            return b+(a-c)/2;
        case 7:
            return (a+b)/2;
        default:
            cerr << "Error: invalid prediction mode\n";
            return 0;
    }
}

void imgCodec::encode(string img_path, string code_path) {
    Mat img_in = imread(img_path, IMREAD_GRAYSCALE);
    GolombStream gstream {golomb, code_path};

    // CREATE HEADER FOR ENCODED VALUE
    for (int i = 24; i >= 0; i-=8)
        gstream.getStream().writeAByte(img_in.cols >> i);       // [0-3] -> nCols
    for (int i = 24; i >= 0; i-=8)
        gstream.getStream().writeAByte(img_in.rows >> i);       // [4-7] -> nRows
    gstream.getStream().writeAByte(img_in.elemSize1());         // [8]   -> Bytes per element

    uint_fast8_t a, b, c, value;
    uchar mode;
    for (size_t col = 0; col < img_in.cols; col++) {
        if (col == 0)                                           // first column -> only b exists
            mode = 2;
        else
            mode = pred_mode;
        for (size_t row = 0; row < img_in.rows; row++) {
            if (row == 0) {                                     // first row -> only a exists
                if (mode == 2)
                    mode == 0;
                else
                    mode = 1;
            }
            value = img_in.at<uint_fast8_t>(col, row);                       // get value at (row, col)
            gstream.encodeNext(abs(value - predicted_value(a, b, c, mode)));      // encode residual: value - pred_value

        }

    }
}