#include "bitstream.h"

using namespace std;

// USED FOR TESTING EX. 5

int main(int argc, char *argv[]) {

    BitStream f{};
    
    fstream file("chars.txt");

    //test writeBit
    f.writeBit(file, 1, 0);
    f.writeBit(file, 1, 5);
    f.writeBit(file, 0, 1);
    f.writeBit(file, 0, 7);    

    // test readBit
    file.seekg(0, file.end);
    size_t charcount = file.tellg();
    file.seekg(0, file.beg);
    cout << "charcount: " << charcount << '\n';

    for (int i = 0; i < charcount; i++) {
        cout << file.peek() << ": ";
        for (int j = 0; j < 8; j++) {
            file.seekg(i, file.beg);
            cout << f.readBit(file, j);
        }
        cout << '\n';
    }

    file.close();

    

    return 0;
}