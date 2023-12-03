#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <filesystem>

using namespace std;

#ifndef BITSTREAM
#define BITSTREAM
    class BitStream {
        private:
            fstream file;
            size_t readPos;
            size_t writePos;
            uintmax_t length;
            uintmax_t bytesRead;

            enum MASK {
                    bit0 = 0b10000000,
                    bit1 = 0b01000000,
                    bit2 = 0b00100000,
                    bit3 = 0b00010000,
                    bit4 = 0b00001000,
                    bit5 = 0b00000100,
                    bit6 = 0b00000010,
                    bit7 = 0b00000001,
                };

        public:
            BitStream() = default;
            BitStream(string path);         // constructor
            bool hasNext();
            bool readBit();
            void writeBit(bool bit);
            vector<bool> readNBits(size_t N);
            void writeNBits(vector<bool> bits);
            string readString();
            void writeString(string str);
            void close();

    };
#endif