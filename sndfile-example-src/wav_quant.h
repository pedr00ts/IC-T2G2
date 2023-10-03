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
        WAVQuant(const size_t sampleSize, const size_t newSampleSize) {
            k = pow(2, sampleSize - newSampleSize);
        }

        // Atualiza counts de acordo com o size fornecido
        std::vector<short>& quant(const std::vector<short>& samples) {
            quantSample.clear();
            quantSample.resize(samples.size());
            std::cout << "k = " << k << '\n';
            for (auto s : samples) {
                std::cout << s << " " << (s/k)*k << '\n';
                quantSample.push_back((s/k)*k);       // incrementa a frequencia adaptada no canal n; incrementa n 
                //std::cout << s/k << " " << s << " \n";;
            }
            std::cout << quantSample.size();
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