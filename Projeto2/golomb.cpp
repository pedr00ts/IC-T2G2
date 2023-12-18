#include "golomb.h"

Golomb::Golomb(uint32_t mod, bool b) {
    if (mod == 0) {
        throw invalid_argument("Error in Golomb constructor: mod should be a positive integer\n");
    }
    m = mod;
    mode = b;
    bitcount = ceil(log2(mod));
    p = pow(2, bitcount);
}

vector<bool> Golomb::encodePrefix(int n) {     // returns unary prefix
    vector<bool> vec {};

    if (mode){
        n *= 2;
        if (n < 0)
            n++;
    }

    for (uint32_t i = 0; i < abs(n)/m; i++)    // insert n/m 0's in vector
        vec.push_back(0);
    vec.push_back(1);                               // insert terminator 1

    return vec;
}

vector<bool> Golomb::encodeSuffix(int n) {     // returns suffix
    vector<bool> vec{};
    
    if (mode) {
        n *= 2;
        if (n < 0)
            n++;
    }

    u_int32_t rem = abs(n) % m;                // remainder used in suffix
    
    uint8_t mask = 0b1;
    uint32_t p = pow(2, bitcount);
    uint8_t ext_bits;
    if (p == m || rem < p-m) {                      // cases: m is a power of 2 or no extended bitcount
        for (int i = bitcount-1; i >= 0; i--) 
            vec.push_back((mask << i) & rem); 
    } else {                                        // rem higher than p
        ext_bits = rem+p-m;
        for (int i = bitcount; i >= 0; i++)
            vec.push_back((mask << i) & ext_bits); 
    }

    if (mode == 0) {                                // insert sign bit
        if (n < 0)
            vec.push_back(1);
        if (n > 0)
            vec.push_back(0);
    }

    return vec;
}

vector<bool> Golomb::encode(int n) {
    vector<bool> p = encodePrefix(n);
    vector<bool> s = encodeSuffix(n);
    p.insert(p.end(), s.begin(), s.end());
    return p;
}

int Golomb::decode(vector<bool> encodedN) {
    uint32_t q {};                                                       // quotient
    uint32_t r {};                                                       // remainder
    int i = 0;                                                           // vector counter
    
    while (encodedN[i++] != 1 && i < (int)encodedN.size()) {             // decode prefix
        q++;
    }
    
    for (int c = 0; c < bitcount && i < (int)encodedN.size(); c++) {     // decode suffix
        r = r << 1;
        r = r | encodedN[i++];
    } 

    u_int32_t p = pow(2, bitcount);
    if (r == p-1 && m != p){                                        // decode extended suffix bits
        while (encodedN[i++] != 0 && i < (int)encodedN.size()) {
            r++;  
        }
    }  

    if (r >= m)
        throw invalid_argument("Value is not coded correctly - suffix is too large\n");

    int value = q*m + r;
    if (mode == 0) {
        if (i < (int)encodedN.size()-1)
            throw invalid_argument("Value is not coded correctly - suffix is too large\n");
        if (q != 0 || r != 0){
            int sign = 1;
            if (encodedN[encodedN.size()-1])
                sign = -1;
            value *= sign;
        }
    } else {
        if (i < (int)encodedN.size())
            throw invalid_argument("Value is not coded correctly - suffix is too large\n");
        if (value % 2 != 0) {
            value++;
            value = -value; 
        }
        value /= 2;
    }

    return value;

}

void Golomb::encodeNext(BitStream& stream, int n) {
    if (mode) {
        n *= 2;
        if (n < 0)
            n++;
    }

    // encode prefix
    for (uint32_t i = 0; i < abs(n)/m; i++) {    // insert n/m 0's in vector
        stream.writeBit(0);
    }
    stream.writeBit(1);                        // insert terminator 1

    // encode suffix
    u_int32_t rem = abs(n) % m;            // remainder used in suffix
    uint8_t ext_bits;
    if (m == p || rem < p-m) {                      // cases: m is a power of 2 or no extended bitcount
        for (uint32_t mask = 1<<(bitcount-1); mask > 0; mask = mask>>1) {
            stream.writeBit((bool) (rem & mask)); 
        }
    } else {                                        // rem higher than p
        ext_bits = rem+p-m;
        for (uint32_t mask = 1<<(bitcount); mask > 0; mask = mask>>1)
            stream.writeBit((bool) (ext_bits & mask));
    }
    // encode sign (when necessary)
    if (!mode) {                                // insert sign bit
        if (n < 0)
            stream.writeBit(1);
        else if (n > 0)
            stream.writeBit(0);
    }
}

bool Golomb::decodeNext(BitStream& stream, int& n) {
    uint32_t q {};                                                       // quotient
    uint32_t r {};                                                       // remainder
    
    // decode prefix
    bool complete = true;
    while ((complete = stream.hasNext()) && stream.readBit() != 1) {
        q++;
    }
    // decode suffix
    for (int c = 0; (complete = stream.hasNext()) && c < bitcount; c++) {
        r = r << 1;
        r = r | stream.readBit();
    } 
    // decode extended suffix bits (when expected)
    if (m != p && r >= p-m) {
        while ((complete = stream.hasNext()) && stream.readBit() != 0 && r < m) {
            r++;  
        }
    }
    // verify incomplete code
    if (!complete)
        return false;               // value was not completely decoded
    // verify suffix overflow error
    if (r >= m)
        throw invalid_argument("Value is not coded correctly - suffix is too large\n");

    // calc decoded value
    int value = q*m + r;
    if (mode == 0) {
        if (q != 0 || r != 0){                  // expecting sign bit
            if (!stream.hasNext())
                throw invalid_argument("Value is not coded correctly - missing sign bit\n");
            bool isNegative = stream.readBit();
            if (isNegative)
                value = -value;
        }
    } else {
        if (value % 2 != 0) {
            value++;
            value = -value; 
        }
        value /= 2;
    }
    n = value;          // write decoded value to the given reference

    return true;        // value was completely decoded
}

uint32_t Golomb::M() {
    return m;
}

bool Golomb::Mode() {
    return mode;
}

uint8_t Golomb::BitCount() {
    return bitcount;
}

uint32_t Golomb::P() {
    return p;
}


// GolombStream
GolombStream::GolombStream(Golomb golomb, string path) {
    GolombStream::golomb = golomb;
    GolombStream::stream = BitStream{path};
}

bool GolombStream::hasNext() {
    return stream.hasNext();
}

void GolombStream::close() {
    stream.close();
}

void GolombStream::encodeNext(int n) {
    if (golomb.Mode()) {
        n *= 2;
        if (n < 0)
            n++;
    }

    // encode prefix
    for (uint32_t i = 0; i < abs(n)/golomb.M(); i++) {    // insert n/m 0's in vector
        stream.writeBit(0);
    }
    stream.writeBit(1);                        // insert terminator 1

    // encode suffix
    u_int32_t rem = abs(n) % golomb.M();            // remainder used in suffix
    if (golomb.M() == golomb.P() || rem < golomb.P()-1) {                      // cases: m is a power of 2 or no extended bitcount
        for (uint32_t mask = 1<<(golomb.BitCount()-1); mask > 0; mask = mask>>1) {
            stream.writeBit((bool) (rem & mask)); 
        }
    } else {                                        // rem higher than p
        for (uint8_t i = 0; i < golomb.BitCount(); i++) {      // insert bitcount 1's
            stream.writeBit(1);
        }
        for (u_int32_t i = golomb.P()-1; i < rem; i++) {     // extended 1's
            stream.writeBit(1);
        }
        stream.writeBit(0);                         // value ends in 0
    }
    // encode sign (when necessary)
    if (!golomb.Mode()) {                                // insert sign bit
        if (n < 0) {
            stream.writeBit(1);
        }
        else if (n > 0) {
            stream.writeBit(0);
        }
    }
}

void GolombStream::encodeNext(uint n) {
    // encode prefix
    for (uint32_t i = 0; i < n/golomb.M(); i++) {    // insert n/m 0's in vector
        stream.writeBit(0);
    }
    stream.writeBit(1);                        // insert terminator 1

    // encode suffix
    u_int32_t rem = n % golomb.M();            // remainder used in suffix
    if (golomb.M() == golomb.P() || rem < golomb.P()-1) {                      // cases: m is a power of 2 or no extended bitcount
        for (uint32_t mask = 1<<(golomb.BitCount()-1); mask > 0; mask = mask>>1) {
            stream.writeBit((bool) (rem & mask)); 
        }
    } else {                                        // rem higher than p
        for (uint8_t i = 0; i < golomb.BitCount(); i++) {      // insert bitcount 1's
            stream.writeBit(1);
        }
        for (u_int32_t i = golomb.P()-1; i < rem; i++) {     // extended 1's
            stream.writeBit(1);
        }
        stream.writeBit(0);                         // value ends in 0
    }
}

int GolombStream::decodeNext() {
    uint32_t q {};                                                       // quotient
    uint32_t r {};                                                       // remainder
    
    // decode prefix
    bool complete = true;
    while ((complete = stream.hasNext()) && stream.readBit() != 1) {
        q++;
    }
    // decode suffix
    for (int c = 0; (complete = stream.hasNext()) && c < golomb.BitCount(); c++) {
        r = r << 1;
        r = r | stream.readBit();
    }
    // decode extended suffix bits (when expected)
    if (golomb.M() != golomb.P() && r >= (golomb.P()-golomb.M())) {
        r = r << 1;
        r = r | stream.readBit();
        r = r - golomb.P() + golomb.M();
    }
    // verify incomplete code error
    if (!complete)
        throw invalid_argument("Incomplete code\n");
    // verify suffix overflow error
    if (r >= golomb.M())
        throw invalid_argument("Value is not coded correctly - suffix is too large\n");

    int value = q*golomb.M() + r;
    if (golomb.Mode() == 0) {
        if (q != 0 || r != 0){                  // expecting sign bit
            if (!stream.hasNext())
                throw invalid_argument("Value is not coded correctly - missing sign bit\n");
            bool isNegative = stream.readBit();
            if (isNegative)
                value = -value;
        }
    } else {
        if (value % 2 != 0) {
            value++;
            value = -value; 
        }
        value /= 2;
    }

    return value;
}

uint GolombStream::decodeNextPos() {
    uint32_t q {};                                                       // quotient
    uint32_t r {};                                                       // remainder
    
    // decode prefix
    bool complete = true;
    while ((complete = stream.hasNext()) && stream.readBit() != 1) {
        q++;
    }
    // decode suffix
    for (int c = 0; (complete = stream.hasNext()) && c < golomb.BitCount(); c++) {
        r = r << 1;
        r = r | stream.readBit();
    } 
    // decode extended suffix bits (when expected)
    if (golomb.M() != golomb.P() && r == golomb.P()-1) {
        while ((complete = stream.hasNext()) && stream.readBit() != 0 && r < golomb.M()) {
            r++;  
        }
    }
    // verify incomplete code error
    if (!complete)
        throw invalid_argument("Incomplete code\n");
    // verify suffix overflow error
    if (r >= golomb.M())
        throw invalid_argument("Value is not coded correctly - suffix is too large\n");

    uint value = q*golomb.M() + r;
    return value;
}

BitStream& GolombStream::getStream() {
    return stream;
}