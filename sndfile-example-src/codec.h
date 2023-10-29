#include "bitstream.h"

class Codec {

private:

public:

    Codec() {}

    // DCT Transformation
    vector<short> performDCT(const vector<short>& input)
    {
        size_t N = input.size();
        vector<short> output(N);

        for (size_t  k = 0; k < N; ++k) {
            double sum = 0.0;
            for (size_t n = 0; n < N; ++n) {
                double theta = (n + 0.5) * k * M_PI / N;
                sum += input[n] * cos(theta);
            }
            output[k] = static_cast<short>(sum);
        }

        return output;
    }

    // Inverse DCT
    vector<short> performIDCT(const vector<short>& input) {
        size_t N = input.size();
        vector<short> output(N);

        for (size_t n = 0; n < N; ++n) {
            double sum = 0.0;
            for (size_t k = 0; k < N; ++k) {
                double theta = (n + 0.5) * k * M_PI / N;
                sum += input[k] * cos(theta);
            }
            output[n] = sum;
        }

        return output;
    }

    // Quantization of DCT'ed values
    vector<bool> quantize(const vector<short>& dctCoeffs, size_t nBits) {
        BitStream b{};
        vector<bool> quantized;

        size_t k;
        for (short coeff : dctCoeffs) {
            k = pow(2, (16 - nBits));
            coeff = (coeff/k) * k;
            quantized.push_back(coeff);
        }

        return quantized;
    }

    vector<short> dequantize(const vector<bool>& bits, size_t nBits) {
    // Dequantization of DCT'ed val
        vector<short> dequantized;

        short value = 0;
        for (size_t i = 1; i <= bits.size(); i++) {
            value = value << 1 | bits[i-1];
            if (i % nBits == 0) {
                dequantized.push_back(value);
                value = 0;
            }
        }
        return dequantized;
    }

    // Returns compressed audio input quantified to nBits
    vector<bool> compress(const vector<short>& input, size_t nBits) {
        vector<bool> output = quantize(performDCT(input), nBits);
        return output;
    } 

    // Returns audio quantified in nBits to decompressed vector
    vector<short> decompress(const vector<bool>& input, size_t nBits) {
        vector<short> output = performIDCT(dequantize(input, nBits));
        return output;
    }



};



