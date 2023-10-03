#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <input file> <sampleSize> <channel>\n";
}

int main(int argc, char *argv[]) {

	if(argc != 4) {
		cerr << "Error: wrong number of args\n";
		usage(argv);
		return 1;
	}

	SndfileHandle sndFile { argv[1] };
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

	size_t channel = stoi(argv[3]);
    if(channel >= sndFile.channels()) {
		cerr << "Error: invalid channel requested\n";
		return 1;
	}

    size_t outSampleSize = stoi(argv[2]);
    size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVQuant quantfile { sndFile, 16 };

    while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		quantfile.update(samples, outSampleSize);
	}
    
    quantfile.dump(channel);
    
    return 0;
}