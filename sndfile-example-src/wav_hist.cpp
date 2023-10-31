#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

/**
 * Outputs the usage of this program.
*/
void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <input file> <option>\n";
	cerr << "	List of available options:\n";
	cerr << "	-c <channel> [k]  for desired channel and optional constant k for bins with 2^k values\n";
	cerr << "	-m            for MID\n";
	cerr << "	-s            for SIDE\n";
}

/**
 * Outputs the histogram of a WAV audio file from a list of available histogram options.
*/
int main(int argc, char *argv[]) {

	if(argc < 3 || argc > 5) {
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
	
	// validar opcao selecionada
	string option { argv[2] };

	WAVHist::Type type { };
	int channel, k { };
	if (option == "-c") {
		if (argc < 4) {
			cerr << "Error: missing desired channel\n";
			usage(argv);
			return 1;
		}

		channel = stoi(argv[3]);
		if(channel >= sndFile.channels()) {
			cerr << "Error: invalid channel requested\n";
			return 1;
		}

		k = 0;
		if (argc == 5) {
			k = stoi(argv[4]);
			if(k < 0 || k > 16) {
				cerr << "Error: invalid constant k requested\n";
				return 1;
			}
		}

		type = WAVHist::Type::CHANNELS;
	} else if (option == "-m") {
		type = WAVHist::Type::MID;
	} else if(option == "-s") {
		type = WAVHist::Type::SIDE;
	} else if(option == "-h") {
		usage(argv);
		return 0;
	} else {
		cerr << "Error: invalid option.\n";
		usage(argv);
		return 1;
	}


	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile, type, k };
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		hist.update(samples);
	}

	if (type == WAVHist::Type::CHANNELS)
		hist.dump_counts(channel);
	else
		hist.dump();
	return 0;
}

