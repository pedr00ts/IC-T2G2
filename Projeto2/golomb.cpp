#include "golomb.h"

Golomb::Golomb(uint32_t mod, bool b) {
    if (mod == 0) {
        throw invalid_argument("Error in Golomb constructor: mod should be a positive integer\n");
    }
    m = mod;
    mode = b;
    bitcount = floor(log2(mod));
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
    if (p == m || rem < p-1) {                      // cases: m is a power of 2 or no extended bitcount
        for (int i = bitcount-1; i >= 0; i--) 
            vec.push_back((mask << i) & rem); 
    } else {                                        // rem higher than p
        for (int i = 0; i < bitcount; i++)          // insert bitcount 1's
            vec.push_back(1);

        for (u_int32_t i = p-1; i < rem; i++) {     // extended 1's
            vec.push_back(1);
        }
        
        vec.push_back(0);                           // value ends in 0
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
/*
int main(int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    Golomb G = Golomb{(uint32_t)stoi(argv[1]), (bool)stoi(argv[2])};       // mod, type
    vector<bool> code = G.encode(stoi(argv[3]));
    for (bool b : code) {
        cout << b;
    }
    cout << '\n' << G.decode(code) << '\n';
}
*/