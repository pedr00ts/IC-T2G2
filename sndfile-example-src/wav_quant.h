#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sndfile.hh>
#include "wav_hist.h"

class WAV_Quant {

    private:
        std::vector<std::map<short, size_t>> counts;
        size_t sampleSize;

    public:
        WAV_Quant(const SndfileHandle& sfh) {
            counts.resize(sfh.channels());
            WAV_Quant::sampleSize = 16;
        }

        WAV_Quant(const SndfileHandle& sfh, const size_t sampleSize) {
            counts.resize(sfh.channels());
            WAV_Quant::sampleSize = sampleSize;
        }

        void update(const SndfileHandle& sfh) {
            size_t n = 0;
        }






};