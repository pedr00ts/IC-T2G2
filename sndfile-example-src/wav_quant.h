#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sndfile.hh>
#include "wav_hist.h"

class WAVQuant {

    private:
        std::vector<std::map<short, size_t>> counts;
        size_t sampleSize;

    public:

        // Construtor sem sampleSize do audio original -- default = 16
        WAVQuant(const SndfileHandle& sfh) {
            counts.resize(sfh.channels());
            WAVQuant::sampleSize = 16;
        }

        // Construtor com sampleSize fornecido
        WAVQuant(const SndfileHandle& sfh, const size_t sampleSize) {
            counts.resize(sfh.channels());
            WAVQuant::sampleSize = sampleSize;
        }

        // Atualiza counts de acordo com o size fornecido
        void update(const std::vector<short>& samples, short newSampleSize) {
            if ((newSampleSize < 1) || (newSampleSize > sampleSize)) {
                std::cerr << "Error: desired sampleSize is invalid\n";
                return;
            }
            size_t k = pow(2, sampleSize - newSampleSize);   // constante para alterar a base da amostra 
            size_t n = 0;                                    // contador para os channels
            for (auto s : samples) {
                counts[n++ % counts.size()][s/k]++;          // incrementa a frequencia adaptada no canal n; incrementa n 
            }
        }

        // Dump to stdout
        void dump(const int channel) const {
            for(auto [value, counter] : counts[channel])
			    std::cout << value << '\t' << counter << '\n';
        }

       /* void write_to_wav() {
            
        } */
            
        };