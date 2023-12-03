#include "bitstream.h"

int main(int argc, char* argv[]) {
    if (argc != 2){
        cerr << "wrong arg number\n";
        return 1;
    }
    BitStream bs {argv[1]};
    while(bs.hasNext())
        cout << bs.readBit();
    bs.close();
    return 0;
}