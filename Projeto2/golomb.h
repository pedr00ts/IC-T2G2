#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "bitstream.h"

using namespace std;

#ifndef GOLOMB
#define GOLOMB
class Golomb {
    protected:
        uint32_t m;                                    // encoding modulus
        bool mode;                                     // (0) -> magnitude + sign; (1) -> pos/neg interleaving
        uint8_t bitcount;                              // number of bits in suffix
        uint32_t p;                                    // number of bitcount values
        vector<bool> encodePrefix(int n);              // returns unary prefix
        vector<bool> encodeSuffix(int n);              // returns suffix

    public:
        Golomb(uint32_t mod=4, bool b=0);
        vector<bool> encode(int n);
        int decode(vector<bool> encodedN);
        void encodeNext(BitStream& stream, int n);
        bool decodeNext(BitStream& stream, int& n);
        uint32_t M();
        bool Mode();
        uint8_t BitCount();
        uint32_t P();
};
#endif

#ifndef GOLOMBSTREAM
#define GOLOMBSTREAM
class GolombStream {
    protected:
        Golomb golomb;                                 // golomb code parameters
        BitStream stream;                              // golomb code stream

    public:
        GolombStream(Golomb golomb, string path);
        bool hasNext();
        void close();
        int decodeNext();
        uint_fast8_t decodeNextPos();                  // optimized when input >= 0
        void encodeNext(int n);
        void encodeNext(uint_fast8_t n);               // optimized when input >= 0
        BitStream& getStream();
};
#endif