#include "bitstream.h"

using namespace std;

// USED FOR TESTING EX. 5

void testWrite() {
    BitStream bs {};
    fstream testfile("test.txt");
    vector<bool> bits(56);
    testfile.seekp(testfile.beg);
    for (char i = 'A'; i < 'G'; i++) {
        /*for (size_t j = 0; j < 8; j++) {
            switch(j) {
                case 0:
                    bits[j] = bits[j] & BitStream::bit7;
                    break;
                case 1:
                    bits[j] = bits[j] & BitStream::bit6;
                    break;
                case 2:
                    bits[j] = bits[j] & BitStream::bit5;
                    break;
                case 3:
                    bits[j] = bits[j] & BitStream::bit4;
                    break;
                case 4:
                    bits[j] = bits[j] & BitStream::bit3;
                    break;
                case 5:
                    bits[j] = bits[j] & BitStream::bit2;
                    break;
                case 6:
                    bits[j] = bits[j] & BitStream::bit1;
                    break;
                case 7:
                    bits[j] = bits[j] & BitStream::bit0;
            }
            
        }*/
        //bs.writeByte(testfile, i);
    }
    
    //bs.writeNBits(testfile, bits, 0);

    testfile.close();
}

int main(int argc, char *argv[]) {

    testWrite();

    BitStream f{};
    
    fstream file("chars.txt");

    //test writeBit
    f.writeBit(file, 1, 0);
    f.writeBit(file, 1, 5);
    f.writeBit(file, 0, 1);
    f.writeBit(file, 0, 7);
    f.writeBit(file, 1, 8);
    f.writeBit(file, 1, 1);   

    file.seekg(0, file.end);
    size_t charcount = file.tellg();
    file.seekg(0, file.beg); 

    // test readBit
    cout << "charcount: " << charcount << '\n';

    for (int i = 0; i < charcount; i++) {
        cout << file.peek() << ": ";
        for (int j = 0; j < 8; j++) {
            file.seekg(i, file.beg);
            cout << f.readBit(file, j);
        }
        cout << '\n';
    }

    file.seekg(file.beg);
    vector readValues = f.readNBits(file, 64, 0);
    for (auto value : readValues)
        cout << value;

    file.close();

    

    return 0;
}
