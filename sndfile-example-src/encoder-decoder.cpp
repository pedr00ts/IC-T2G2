#define MAX_BUFFER 64

#include "bitstream.h"

using namespace std;

void usage(char* argv[]) {
    cerr << "Usage: " << argv[0] << " <inputFile> <-d|-e> <outputFile>\n";
}

int decode(char* argv[]) {
    BitStream f{};
    fstream fileIn (argv[1]);
    fstream fileOut (argv[3]);

    fileIn.seekg (0, fileIn.end);
    size_t charcount = fileIn.tellg();
    fileIn.seekg (0, fileIn.beg);

    cout << "charcount: " << charcount << '\n';    

    vector<bool> values(8);
    char* one = "1";
    char* zero = "0";

    for (size_t i = 0; i < charcount; i++) {
        if (charcount < 8)
            values = f.readNBits(fileIn, charcount, 0);    // read remaining bits
        else
            values = f.readNBits(fileIn, 8, 0);            // read 8 bits
        for (bool bit : values) {
            if (bit)
                fileOut.write(one, 1);
            else
                fileOut.write(zero, 1);    
        }
    }

    fileIn.close();
    fileOut.close();

    return 0;

}

int encode(char* argv[]) {
    BitStream f{};
    fstream fileIn(argv[1]);
    fstream fileOut(argv[3]);

    fileIn.seekg (0, fileIn.end);
    size_t charcount = fileIn.tellg();
    fileIn.seekg (0, fileIn.beg);

    vector<char> bits(8);
    //fileOut.seekp(fileOut.beg); 

    while(charcount > 0) {
        if (charcount < 8) {
            bits = f.readNBytes(fileIn, charcount);
            bits.resize(charcount);
            //cout << "read " << charcount << " bytes\n";
        } else {
            bits = f.readNBytes(fileIn, 8);
            //cout << "read " << 8 << " bytes\n";
        }

        char byte = 0x00;
        cout << '\n';
        for (size_t i = 0; i < bits.size(); i++) {
            //cout << "bit value: " << bit << '\n';
            cout << bits[i]; 
            if (bits[i] == '1') {
                byte = byte | 0x01;
            }
            if (i < (bits.size()-1))
                byte = byte << 1;
        }

        if ((bits.size() % 8) != 0) {
            byte = byte << (8-bits.size());
        }

        fileOut.write(&byte, 1);
        charcount -= bits.size();
    }

    fileIn.close();
    fileOut.close();

    return 0;
}

int main (int argc, char* argv[]) {

    if(argc != 4) {
        cerr << "Error: invalid number of arguments\n";
        usage(argv);
        return 1;
    }

    if (argv[2][1] == 'd') {
        decode(argv);
    } else if (argv[2][1] == 'e') {
        encode(argv);
    } else {
        cerr << "Invalid operation\n";
        usage(argv);
        return 1;
    }

    

    return 0;
}