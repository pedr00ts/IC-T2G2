#ifndef WAVHIST_H
#define WAVHIST_H
#define SHRT_MIN -32768
#define SHRT_MAX 32767

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> mid;				 // histograma com a média dos valores em counts. Não é vector porque mid só tem 1 canal
	std::map<short, size_t> side;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}


//	Atualiza o histograma mid de acordo com os valores em counts.
	void update_mid() {
		if (counts.size() == 1) {
			mid = counts[0];
		} else {
			short sum;
			for (short i = SHRT_MIN; i <= SHRT_MAX; i++) {
				sum = 0;
				for (short c = 0; c < counts.size(); c++) {
					sum += counts[c][i];
				}
				mid[i] = sum/counts.size();
			}

		}		

	}

	void update_side() {
		if (counts.size() != 2) {
			std::cout << "Error in update_side: function requires two channels" + '\n';
		} else {
			for (short i = SHRT_MIN; i <= SHRT_MAX; i++) {
				side[i] = counts[0][i] - counts[1][i];
				
			}
		}
	}

	/*void update_mid(const std::vector<short>& samples) {
		float sum;
		for (auto s : samples){

			sum = 0;
			for (int i = 1; i <= counts.size(); i++) {
				sum += s;
			}
		}
		counts[0][round(sum/counts.size())]++;

			
	} */



	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])
			std::cout << value << '\t' << counter << '\n';
	}

// Escreve no stdout os valores atuais de mid. 
	void dump_mid() const {
		for(auto [value, counter] : mid)
			std::cout << value << '\t' << counter << '\n';
	}

// Escreve no stdout os valores atuais de side. 
	void dump_side() const {
		for(auto [value, counter] : side)
			std::cout << value << '\t' << counter << '\n';
	}	
};

#endif

