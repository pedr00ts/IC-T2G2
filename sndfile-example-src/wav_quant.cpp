#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <input file> <sampleSize> <output file> <newSampleSize>\n";
}

int main(int argc, char *argv[]) {

	if(argc != 5) {
		cerr << "Error: wrong number of args\n";
		usage(argv);
		return 1;
	}

	SndfileHandle sndFile { argv[1] };
	SndfileHandle sndFileOut = SndfileHandle(argv[3], SFM_WRITE, sndFile.format(), sndFile.channels(), sndFile.samplerate());
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	short sampleSize = stoi(argv[2]);
	short newSampleSize = stoi(argv[4]);

	if (newSampleSize < 1 || newSampleSize > sampleSize || sampleSize < 1 ) {
            std::cerr << "Error: desired sample size is invalid";
            return 1;
    }

  size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	
	WAVQuant quantfile { sampleSize, newSampleSize };

    while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		cout << quantfile.quant(samples).data()[0];
		sndFileOut.writef(quantfile.quant(samples).data(), nFrames);
	}
    
    return 0;
}	