#include "bitstream.h"

using namespace std;

int main(int argc, char *argv[]) {

    BitStream f{};
    
    fstream readFile("chars.txt");
    fstream writeFile("chars.txt");

    //test writeBit
    f.writeBit(writeFile, 1, 0);
    f.writeBit(writeFile, 1, 5);
    writeFile.close();
    

    // test readBit
    readFile.seekg(0, readFile.end);
    size_t charcount = readFile.tellg();
    readFile.seekg(0, readFile.beg);
    cout << "charcount: " << charcount << '\n';

    for (int i = 0; i < charcount; i++) {
        cout << readFile.peek() << ": ";
        for (int j = 0; j < 8; j++) {
            readFile.seekg(i, readFile.beg);
            cout << f.readBit(readFile, j);
        }
        cout << '\n';
    }

    

    return 0;
}