#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class BitStream {
    private:
        fstream file;
        size_t readPos;
        size_t writePos;

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
        BitStream(string path) {
            file = fstream{path};
            file.seekg(file.beg);   // read: first byte
            readPos = 0;
            file.seekp(file.end);   // write: last byte
            writePos = 0;
        }

        bool readBit() {
            char B = file.get();
            bool bit = 0;
            switch(readPos++) {
                case 0:
                    bit = B & bit0;
                    break;
                case 1:
                    bit = B & bit1;
                    break;
                case 2:
                    bit = B & bit2;
                    break;
                case 3:
                    bit = B & bit3;
                    break;
                case 4:
                    bit = B & bit4;
                    break;
                case 5:
                    bit = B & bit5;
                    break;
                case 6:
                    bit = B & bit6;
                    break;
                case 7:
                    bit = B & bit7;
                    break;
            }
            if (readPos < 8) {
                file.seekg(file.cur-1);
            } else {
                readPos = 0;
            }
            
            return bit;
        }

        void writeBit(bool bit) {
            char B = 0x00;          // Initialize byte

            if (writePos != 0) {    // Pos not aligned -> read last byte
                int rp = file.tellg();
                file.seekg(-1, file.end);
                B = file.peek();
                file.seekg(rp);
           }
            
            if(bit) {               // Modify byte
                B = B | (MASK::bit0 >> writePos);
            } else { 
                B = B & (0xFF7F >> writePos);
            }

            if (writePos == 0) {
                file.seekp(0, file.end);
            } else {
                file.seekp(-1, file.end);
            }
            file.put(B);            // Write byte

            writePos = (writePos + 1) % 8;           
        }

        vector<bool> readNBits(size_t N) {
            if (N > 64) {
                throw std::invalid_argument("invalid N value in readNBits. [0 <= N <= 64]\n");
            }
            vector<bool> bits{};
            for(int i = 0; i < N; i++) 
                bits.push_back(readBit());
            
            return bits; 
        }

        void close() {
            file.close();
        }



        
        vector<bool> readNBits(size_t N);
        void writeNBits(vector<bool> bits);
        string readString();
        void writeString(char* s);

};

int main(int argc, char* argv[]) {
    BitStream testfile {"test.txt"};


    cout << "Primeiros 6 bits:\n";
    for(int i = 0; i < 6; i++) {
        cout << testfile.readBit() << " ";
    }

    testfile.writeBit(0);
    testfile.writeBit(1);
    testfile.writeBit(0);
    testfile.writeBit(0);
    testfile.writeBit(0);
    testfile.writeBit(1);
    testfile.writeBit(1);
    testfile.writeBit(1);

    testfile.close();
}

