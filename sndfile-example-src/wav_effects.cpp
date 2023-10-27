#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"
// TODO: include other effects

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <input file> <[-e][-E][-A]> <opt1> <opt2>  <output file>\n";
}

enum EFFECT {
     e = 'e', E = 'E', A = 'A'   
};

int main(int argc, char *argv[]) {

    if(argc != 6) {
            cerr << "Error: wrong number of args\n";
            usage(argv);
            return 1;
    }



    SndfileHandle sndFile { argv[1] };
	SndfileHandle sndFileOut = SndfileHandle(argv[argc-1], SFM_WRITE, sndFile.format(), sndFile.channels(), sndFile.samplerate());
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

	vector<short> samples(sndFile.frames());
    size_t i = 0;
    while(sndFile.readf(&samples.data()[i++], FRAMES_BUFFER_SIZE)); // read all samples

    vector<short> samplesOut;
    switch(argv[2][1]) {
    case e:
        {
            float delay = stof(argv[3]);
            float decay = stof(argv[4]);
            if (delay < 0 || delay > 20000) {
                cerr << "Error: invalid delay value: " << delay << "\n";
                usage(argv);
                return 1;
            }
        
            if (decay < 0 | decay >= 1) {
                cerr << "Error: invalid decay value: " << decay << "\n";
                usage(argv);
                return 1;
            }
            cout << delay << '\n';
            cout << decay << '\n';
            Wav_Echo echo = Wav_Echo{sndFile.samplerate(), delay, decay};
            cout << "echo criado \n";
            samplesOut = echo.apply(samples); 
            cout << samples.size() << '\n';
            cout << samplesOut.size() << '\n';
            sndFileOut.writef(samplesOut.data(), samplesOut.size());
            return 0;
        }
    case 'E':
        {
            float delay = stof(argv[3]);
            float decay = stof(argv[4]);
            if (delay < 0 || delay > 20000) {
                cerr << "Error: invalid delay value: " << delay << "\n";
                usage(argv);
                return 1;
            }


            if (decay < 0 | decay >= 1) {
                cerr << "Error: invalid decay value: " << decay << "\n";
                usage(argv);
                return 1;
            }
            break;
        }
    case 'A':

        break;
    default:
        usage(argv);
        return 1;
    }
    

    
    return 0;
}	


