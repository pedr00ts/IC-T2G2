#include "bitstream.h"

void read(string path) {
    BitStream rs {path};
    while(rs.hasNext())
        cout << rs.readBit();
}

void write(string path, string bits) {
    BitStream ws {path};
    for(auto c: bits) {
        bool b = (bool) stoi(string {c});
        cout << b;
        ws.writeBit(b);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        cerr << "wrong arg number\n";
        return 1;
    }
    
    switch(argv[1][1]) {
        case 'r':
            read(argv[2]);
            break;
        case 'w':
            write(argv[2], argv[3]);
            break;
        default:
            cerr << "invalid operation\n";
    }
    
    return 0;
}