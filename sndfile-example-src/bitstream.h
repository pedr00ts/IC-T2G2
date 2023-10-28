#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class BitStream {
    private:

        // convert variable in bits to corresponding value in bytes.
        // NOT TESTED
        size_t byteFormat(size_t bits) {
            if (bits % 8 == 0) 
                return bits;
            else 
                return (0 << bits/8) & bits;
        }

    public:

        BitStream() {

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

        // reads bit at position pos from the stream
        // requires correct position in the stream
        bool readBit(fstream& fileIn, size_t pos) {

            // read byte from the stream
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

        // reads N bits from the stream, starting at position pos
        vector<bool> readNBits(fstream& fileIn, size_t N, size_t pos) {
            vector<bool> values(N);
            for (int i = 0; i < N; i++) {
                values[i] = readBit(fileIn, pos);
                if (pos < 7)
                    fileIn.seekg(fileIn.tellg()-1);
                pos = (pos+1) % 8;
            }
            return values;
        }

        // writes bit to byte at the end of file, in position pos
        // pos > 7 -> write new byte with value 0x00
        int writeBit(fstream& fileIn, bool bit, size_t pos) {
            // create new byte
            if (pos > 7) {
                //fileIn.seekp(0, fileIn.end);
                fileIn.put(bit << 7);
                return 0;
            } 

            // read final byte
            char* byte = new char[1];           
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

            return 0;
        }

        // 
        int writeNBits(fstream& fileIn, const vector<bool>& values, size_t pos) {
            size_t N = values.size();
            size_t i = 0;

            if(pos > 7) {
                cerr << "Error in writeNBits: invalid pos value\n";
                return 1;
            }

            // adjust byte position
            while(pos != 8) {
                if (i == N)
                    return 0;
                writeBit(fileIn, values[i++], pos++);
            }

            // write byte blocks
            char* byte;
            while((N-i) / 8 > 0) {
                byte[0] = 0x00;
                for (size_t j = 0; j < 7; j++) {
                    byte[0] = (byte[0] << 1) | values[i+j];
                }
                fileIn.write(byte, 1);
                i += 8;
            }

            pos = 0;
            while (i < N) {
                writeBit(fileIn, values[i++], pos++);
            }

            return 0;
        }


        // reads N bytes from provided stream
        vector<char> readNBytes(fstream& fileIn, size_t N) {
            vector<char> values(N);
            for(size_t i = 0; i < N; i++) {
                fileIn.get(values[i]);
            }
            return values;
        }

        // writes bytes to provided stream
        void writeBytes(fstream& fileIn, vector<char> values) {
            fileIn.write(&values[0], values.size());
        }

        // converts byte vector to bit vector (most significant to least)
        vector<bool> decode(vector<char> bytes) {
            vector<bool> bits(bytes.size()*8);
            size_t i = 0;
            size_t mask = 0x80;
            for (char byte : bytes) {
                do {
                    bits[i++] = (mask >> (i % 8)) & byte;
                } while(i % 8 != 0);
            }
            return bits;
        }

        // converts bit vector to byte vector
        vector<char> encode(vector<bool> bits) {
            size_t size = bits.size()/8;
            if (bits.size()%8 != 0)
                size++;
            vector<char> bytes(size);
            char byte = 0x00;
            for (size_t i = 1; i <= size; i++) {
                byte = byte | bits[i];
                if(i % 8 == 0) {
                    bytes[i/8] = byte;
                    byte = 0x00;
                }
                byte << 1;
            }
            if (bits.size() % 8 != 0)
                bytes[size-1] = bytes[size-1] << (7 - bits.size() % 8);
            return bytes;   
        }

        string readString(fstream& fileIn) {
            string res {};
            char* next;
            while (fileIn.peek() != '\0') {
                fileIn.get(next[0]);
                res.append(next); 
            }
            return res;
        }

        void writeString(fstream& fileIn, string s) {
            char c = s[0];
            while(c != '\0')
                fileIn.write(&c, 1);
        }


        




};