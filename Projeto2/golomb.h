#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Golomb {
    private:
        uint32_t m;                                    // encoding modulus
        bool mode;                                     // (0) -> magnitude + sign; (1) -> pos/neg interleaving
        uint8_t bitcount;                              // number of bits in suffix
        vector<bool> encodePrefix(int n);              // returns unary prefix
        vector<bool> encodeSuffix(int n);              // returns suffix

    public:
        Golomb(uint32_t mod=4, bool b=0);
        vector<bool> encode(int n);
        int decode(vector<bool> encodedN);
};