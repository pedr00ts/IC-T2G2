#include "bitstream.h"

void read(BitStream& rs) {
    int c = 0;
    while(rs.hasNext()) {
        cout << rs.readBit();
        if (!(++c % 8))
            cout << ' ';
    }
}

void write(BitStream& ws, string bits) {
    for(auto c: bits) {
        bool b = (bool) (c - '0');
        cout << b;
        ws.writeBit(b);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "wrong arg number\n";
        return 1;
    }

    BitStream bs {argv[2]};
    switch(argv[1][1]) {
        case 'r':
            if (!bs.readOk()) {
                cerr << "bitstream is not readable." << endl;
                return 1;
            }
            read(bs);
            break;
        case 'w':
            if (argc != 4) {
                cerr << "wrong arg number - missing bits to write" << endl;
                return 1;
            }
            if (!bs.writeOk()) {
                cerr << "bitstream is not writable." << endl;
                return 1;
            }
            write(bs, argv[3]);
            break;
        default:
            cerr << "invalid operation\n";
    }
    bs.close();
    
    return 0;
}