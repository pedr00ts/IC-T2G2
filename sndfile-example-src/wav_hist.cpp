#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {

	if(argc < 3 || argc > 4) {
		cerr << "Usage: " << argv[0] << " <input file> <option>\n";
		cerr << "-m for MID";
		cerr << "-s for SIDE";
		cerr << "-c <channel> for desired channel";
		return 1;
	}

	SndfileHandle sndFile { argv[argc-2] };
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

	string option { argv[2] };
	int channel { };
	if (option == "-c") {
		if (argc != 4) {
				cerr << "Error: missing desired channel\n";
				return 1;
			}

		channel = stoi(argv[argc-1]);
		if(channel >= sndFile.channels()) {
			cerr << "Error: invalid channel requested\n";
			return 1;
		}
	} else {
		cerr << "Error: invalid option. Valid options are:\n";
		cerr << "-m for MID";
		cerr << "-s for SIDE";
		cerr << "-c <channel> for desired channel";
		return 1;
	}


	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		
		if (option == "-c") {
			hist.update(samples);
		} else if (option == "-m") {
			//hist.update_mid(samples);
		} else {
			//hist.update_mid(samples);
		}
	}

	if (option == "-c") {
		hist.dump(channel);
	}
	return 0;
}

