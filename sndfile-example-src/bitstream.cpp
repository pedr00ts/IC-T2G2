#include "bitstream.h"

using namespace std;

int main(int argc, char *argv[]) {

    BitStream f{};
    
    ifstream readFile("chars.txt");

    readFile.seekg(0, readFile.end);
    int charcount = readFile.tellg() - 1;
    readFile.seekg(0, readFile.beg);
    cout << charcount << '\n';

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