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
	public:
	enum Type {
		CHANNELS, MID, SIDE
	};

  private:
	std::vector<std::map<short, size_t>> counts;
	std::map<short, size_t> mid;				
	std::map<short, size_t> side;
	Type type;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
		type = Type::CHANNELS;
	}

	WAVHist(const SndfileHandle& sfh, const Type aType) {
		counts.resize(sfh.channels());
		type = aType;
	}

	// Atualiza o histograma counts.
	void update_counts(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	// Atualiza o histograma mid.
	void update_mid(const std::vector<short>& samples) {
		short freq = 0;
		short c = 0;
		for (auto s : samples) {
			freq += s;
			if (c++ % counts.size() == 0) {
				mid[freq/counts.size()]++;
				freq = 0;
			}
		}
	}

	// Atualiza o histograma side.
	void update_side(const std::vector<short>& samples) {
		if (counts.size() != 2)
			std::cout << "Error in update_side: function requires two channels" + '\n';

		short freq = 0;
		short c = 0;
		for (auto s : samples) {
			if (c++ == 0) {
				freq = s;
			} else {
				if (freq >= s) {
					side[(freq-s)/2]++;
				} else {
					side[(s-freq)/2]++;
				}
				c = 0;
			}
		}
	}

	void update(const std::vector<short>& samples) {
		switch (type) {
			case Type::CHANNELS:
				update_counts(samples);
				break;
			case Type::MID:
				update_mid(samples);
				break;
			case Type::SIDE:
				update_side(samples);
				break;
			default:
				break;
		}
	}


	// Escreve no stdout os valores atuais de counts. 
	void dump_counts(const int channel) const {

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

	void dump() const {
		switch (type) {
			case Type::MID:
				dump_mid();
				break;
			case Type::SIDE:
				dump_side();
				break;
			default:
				break;
		}
	}
};

#endif

