#include "bitstream.h"

BitStream::BitStream(string path) {
    file = fstream(path, ios::in | ios::app);   // read from the beginning and write to the end
    readPos = 0;
    writePos = 0;
    bytesRead = 0;
    // check file size (bytes)
    filesystem::path p{path};
    if (filesystem::exists(p))
        size = filesystem::file_size(p);
    else
        size = 0;
    cout << size << " bytes" << endl;
}

bool BitStream::readOk() {
    return file.is_open() && file.good();
}

bool BitStream::writeOk() {
    return file.is_open() && file.good();
}

bool BitStream::hasNext() {
    return (bytesRead < size);
}

bool BitStream::readBit() {
    if (readPos == 0){
        file.get(readByte);             
    }
    
    bool bit = 0;
    switch(readPos) {
        case 0:
            bit = readByte & bit0;
            break;
        case 1:
            bit = readByte & bit1;
            break;
        case 2:
            bit = readByte & bit2;
            break;
        case 3:
            bit = readByte & bit3;
            break;
        case 4:
            bit = readByte & bit4;
            break;
        case 5:
            bit = readByte & bit5;
            break;
        case 6:
            bit = readByte & bit6;
            break;
        case 7:
            bit = readByte & bit7;
            break;
    }
    readPos = (readPos+1) % 8;
    if (!readPos)
        bytesRead++;
    return bit;
}

void BitStream::writeBit(bool bit) {
    writeByte = (writeByte << 1) | bit;

    if (writePos == 7)
        file.put(writeByte);
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
        bytesRead++;
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

// Change to hasNext and update bytesRead 
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
    if (writePos) {
        writeByte = writeByte << (8 - writePos);
        file.put(writeByte);
    }
    file.close();
}
