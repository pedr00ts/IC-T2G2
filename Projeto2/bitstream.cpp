#include "bitstream.h"

BitStream::BitStream(string path) {
    file = fstream{path};
    file.seekg(file.beg);   // read: first byte
    readPos = 0;
    file.seekp(file.end);   // write: last byte
    writePos = 0;
}

bool BitStream::hasNext() {
    return !file.eof();
}

bool BitStream::(size_t N) {
    std::streampos pos = file.cur;
}


bool BitStream::readBit() {
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
        file.seekg(file.cur-1);
        readPos = 0;

    return bit;
}

void BitStream::writeBit(bool bit) {
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

vector<bool> BitStream::readNBits(size_t N) {
    if (N > 64) {
        throw std::invalid_argument("invalid N value in readNBits. [0 <= N <= 64]\n");
    }
    vector<bool> bits{};
    
    char c;
    while (N >= 8) {
        c = file.get();
        for (int i = 7; i >= 0; i++)
            bits.push_back(c & (0b1 << i));
        N -= 8;
    }

    for(size_t i = 0; i < N; i++) 
        bits.push_back(readBit());
    
    return bits; 
}

void BitStream::writeNBits(vector<bool> bits) {
    if (bits.size() > 64) {
        throw std::invalid_argument("invalid bits argument in readNBits. [0 <= bits.size() <= 64]\n");
    }

    size_t n = bits.size()/8;
    char c;
    for (size_t i = 0; i < n; i++) {
        c = 0x00;
        for (size_t j = 0; j < 8; j++) {
            c = c | (bits[8*i + j] << 7-j);
        }
        file.put(c);
    }
    
    for(size_t i = bits.size() - bits.size()%8; i < bits.size(); i++)
        writeBit(bits[i]);
}

string BitStream::readString() {
    vector<char> ch{};
    char next;

    do {
        next = file.peek();
        if ((next != file.eof()) && (next != '\0'))
            ch.push_back(next);
    } while ((next != file.eof()) && (next != '\0'));
    
    string str(ch.begin(), ch.end());
    return str;
}

void BitStream::writeString(string str) {
    for(char c : str)
        file.put(c);
}

void BitStream::close() {
    file.close();
}
