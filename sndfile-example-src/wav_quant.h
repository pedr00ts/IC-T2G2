#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sndfile.hh>
#include "wav_hist.h"

class WAVQuant {

    private:
        short k;               // constante de quantização
        std::vector<short> quantSample;

    public:
        
        // Construtor com sampleSize fornecido
        WAVQuant(const short sampleSize, const short newSampleSize) {
            k = (short)pow(2, sampleSize - newSampleSize);
        }

        // Atualiza counts de acordo com o size fornecido
        std::vector<short>& quant(const std::vector<short>& samples) {
            quantSample.clear();
            //quantSample.resize(samples.size());
            //std::cout << "k = " << k << '\n';
            for (auto s : samples) {
                //std::cout << s << " " << (s/k)*k << '\n';
                quantSample.push_back((s/k)*k);         // adiciona ao fim do vetor a sample alterada
                //std::cout << s/k << " " << s << " \n";;
            }
            //std::cout << quantSample.size();
            return quantSample;
        }

        // Dump to stdout
      //  void dump(const int channel) const {
      //      for(auto [value, counter] : counts[channel])
	//	    std::cout << value << '\t' << counter << '\n';
     //   }

       /* void write_to_wav() {
            
        } */
            
        };