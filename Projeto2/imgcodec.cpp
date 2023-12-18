#include "imgcodec.h"
#include <opencv2/opencv.hpp>

imgCodec::imgCodec(uint32_t m, uchar pred_mode) {            // usar modo unsigned
    golomb = Golomb(m, 0);
    imgCodec::pred_mode = pred_mode;
}

uint_fast8_t imgCodec::predicted_value(uint_fast8_t a, uint_fast8_t b, uint_fast8_t c, uchar mode) {
    switch(mode) {
        case 0:                             // mode 0 -> predicted value is always 0
            return 0;         
        case 1:
            return a;
        case 2:
            return b;
        case 3:
            return c;
        case 4:
            if (c < (a+b))
                return (a+b-c);
            else
                return 0;
        case 5:
            if (c < (2*a + b))
                return (a+(b-c)/2);
            else
                return 0;
        case 6:
            if (c < (2*b + a))
                return (b+(a-c)/2);
            else
                return 0;
        case 7:
            return ((a+b)/2);
        default:
            cerr << "Error: invalid prediction mode\n";
            return 0;
    }
}

void imgCodec::encode(string img_path, string code_path) {
    Mat img_in = imread(img_path, IMREAD_GRAYSCALE);
    GolombStream gstream {golomb, code_path};

    // CREATE HEADER FOR ENCODED FILE
    for (int i = 24; i >= 0; i-=8)
        gstream.getStream().writeAByte(img_in.cols >> i);       // [0-3] -> nCols
    for (int i = 24; i >= 0; i-=8)
        gstream.getStream().writeAByte(img_in.rows >> i);       // [4-7] -> nRows
    gstream.getStream().writeAByte(img_in.elemSize1());         // [8]   -> Bytes per element

    // ENCRYPT IMAGE
    uint_fast8_t a = 0, b = 0, c = 0, value;
    uchar mode;
    for (int col = 0; col < img_in.cols; col++) {
        for (int row = 0; row < img_in.rows; row++) {
            if (col == 0) {
                if (row == 0)   
                    mode = 0;
                else {
                    mode = 2;
                    b = img_in.at<uint_fast8_t>(col, row-1); 
                }
            } else {
                a = img_in.at<uint_fast8_t>(col-1, row);
                if (row == 0)
                    mode = 1;
                else {
                    mode = pred_mode;
                    b = img_in.at<uint_fast8_t>(col, row-1); 
                    c = img_in.at<uint_fast8_t>(col-1, row-1);
                }
            }
            value = img_in.at<uint_fast8_t>(col, row);                                          // get value at (row, col)
            gstream.encodeNext((uint_fast8_t)abs(value - predicted_value(a, b, c, mode)));      // encode residual: value - pred_value
        }
    }
}

void imgCodec::decode(string code_path, string img_path) {
    GolombStream gstream {golomb, code_path};

    // READ HEADER
    char* header = new char(9);
    for (int i = 0; i < 9; i++) {
        header[i] = gstream.getStream().readAByte();
    }
    uint32_t nCols = header[0] << 24 | header[1] << 16 | header[2] << 8 | header[3];
    uint32_t nRows = header[4] << 24 | header[5] << 16 | header[6] << 8 | header[7];
    uchar bytesPerElem = header[8];
    uchar img_type;

    switch(bytesPerElem) {
        case 1:
            img_type = CV_8U;
            break;
        default:
            img_type = CV_16U;
    }

    // DECRYPT IMAGE
    Mat img_out = Mat::zeros(nCols, nRows, img_type);
    uint_fast8_t a = 0, b = 0, c = 0, pred_value;
    uchar mode;
    for (int col = 0; col < img_out.cols; col++) {
        for (int row = 0; row < img_out.rows; row++) {
            if (col == 0) {
                if (row == 0)   
                    mode = 0;
                else {
                    mode = 2;
                    b = img_out.at<uint_fast8_t>(col, row-1); 
                }
            } else {
                a = img_out.at<uint_fast8_t>(col-1, row);
                if (row == 0)
                    mode = 1;
                else {
                    mode = pred_mode;
                    c = img_out.at<uint_fast8_t>(col-1, row-1);
                }
            }
            pred_value = predicted_value(a, b, c, mode);                                 // get predicted value
            img_out.at<uint_fast8_t>(col, row) = gstream.decodeNextPos() + pred_value;   // decode value: residual + pred_value
        }
    }
    imwrite(img_path, img_out);    
    gstream.close();
}