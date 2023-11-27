#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Golomb {
    private:
        uint32_t m;         // encoding modulus
        bool mode;          // (0) -> magnitude + sign; (1) -> pos/neg interleaving
        uint8_t bitcount;   // number of bits in suffix
        
        vector<bool> prefix(int n) {
            vector<bool> vec {};

            if (mode){
                n *= 2;
                if (n < 0){ 
                    n--;
                }
            }

            for (uint32_t i = 0; i < n/m; i++)    // insert n/m 0's in vector
                vec.push_back(0);
            vec.push_back(1);                     // insert terminator 1
    
            return vec;
        }

        vector<bool> suffix(int n) {
            vector<bool> vec{};
            
            if (mode) {
                n *= 2;
                if (n < 0){ 
                    n--;
                }
            }

            u_int32_t rem = ((uint32_t)n) % m;              // remainder used in suffix
            
            if (pow(2, bitcount) == m) {                // m is a power of 2 --> no extended bitcount
                for (uint32_t i = bitcount-1; i >= 0; i--) 
                    vec.push_back(rem >> i);     
            } else {                                        // m not power of 2 --> extended bitcount 
                uint32_t ext_symb = m - pow(2, bitcount);   // number of extended symbols -> total symbols == 2 * ext_symb
                                                            // symbol is extended if rem 
            }

            // INSERT SIGN BIT

            // INCLUDE MODE 1: MULTIPLY N BY 2; ADD 1 IF N < 0
        }


    public:
        Golomb(uint32_t mod, bool b) {
            m = mod;
            mode = b;
            bitcount = floor(log2(mod));
        } 

        

        




};