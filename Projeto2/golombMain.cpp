#include "golomb.h"


void usage(char* argv[]) {
    cout << "Usage: " << argv[0] << " <m> <mode> <number>\n";
    cout << "m -> encoding modulus\n";
    cout << "mode: 0 -> modulus + sign; 1 -> positive/negative interleaving\n";
}

// Prints the given integer in its corresponding Golomb code and then prints then decodes it back
int main(int argc, char* argv[]) {
    if (argc != 4) {
        usage(argv);
        return 1;
    }

    Golomb G = Golomb{(uint32_t)stoi(argv[1]), (bool)stoi(argv[2])};       // mod, type
    vector<bool> code = G.encode(stoi(argv[3]));
    for (bool b : code) {
        cout << b;
    }
    cout << '\n' << G.decode(code) << '\n';
}
