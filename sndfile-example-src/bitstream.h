#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class BitStream {
    private:
//        ifstream fileIn;
//        ofstream fileOut;

        // convert variable in bits to corresponding value in bytes.
        // NOT TESTED
        size_t byteFormat(size_t bits) {
            if (bits % 8 == 0) 
                return bits;
            else 
                return (0 << bits/8) & bits;
        }

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

        BitStream() {

        }

//        void setFileIn(ifstream& inputStream){
//            fileIn = inputStream;
//        }

        // reads bit at position pos from the stream
        // requires correct position in the stream
        bool readBit(fstream& fileIn, size_t pos) {

            // read final byte from the stream
            char byte;
            fileIn.get(byte);
            
            // return the requested bit
            switch(pos) {
                case 0:
                    return byte & bit0;
                case 1:
                    return byte & bit1;
                case 2:
                    return byte & bit2;
                case 3:
                    return byte & bit3;
                case 4:
                    return byte & bit4;
                case 5:
                    return byte & bit5;
                case 6:
                    return byte & bit6;
                case 7:
                    return byte & bit7;
                default:
                    return 0;
            }
            
            cerr << "Error in readBit\n";
            return 1;
        }

        // writes bit to byte at the end of file, in position pos 
        void writeBit(fstream& fileIn, bool bit, size_t pos) {
            char* byte = new char[1];

            // read final byte
            fileIn.seekg(-1, fileIn.end);
            fileIn.get(byte[0]);

            cout << "Original byte: " << byte[0] << '\n';
            
            // modify byte
            if (bit) {
                byte[0] = (0b10000000 >> pos) | byte[0];  
                cout << "Modified byte: " << byte[0] << '\n';
            } else {
                //cout << "bitwise operator: " << (0xF7>>pos) << '\n'; 
                byte[0] = (0b0111111101111111 >> pos) & byte[0]; 
                cout << "Modified byte: " << byte[0] << '\n';
            }

            // write to file
            fileIn.seekp(fileIn.tellg()-1);
            fileIn.write(byte, 1);
        }

        // reads byte from provided stream
        char readByte(ifstream& fileIn) {
            char byte;
            fileIn.get(byte);
            return byte;
        }


        




};