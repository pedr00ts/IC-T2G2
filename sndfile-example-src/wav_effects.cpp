#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"
// TODO: include other effects

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <input file> <[-e][-d][-A]> <opt1> <opt2>  <output file>\n";
}

enum EFFECT {
     e = 'e', d = 'd', A = 'A'   
};

vector<vector<short>> separate(const vector<short>& samples, int channels) {
    vector<vector<short>> sep(channels, vector<short>());
    
    cout << channels;
    for(int i=0; i < samples.size(); i++) {
        sep[i%channels].push_back(samples[i]);
    }

    return sep;
}

vector<short> join(const vector<vector<short>>& sep) {
    int channels = sep.size();
    int frames = sep[0].size();
    vector<short> samples(channels * frames);

    for(int i=0; i < samples.size(); i++) {
        for(int c=0; c<channels; c++) {
            samples[i] = sep[c][(int)(i/channels)];
        }
    }

    return samples;
}

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

	vector<short> samples(sndFile.frames() * sndFile.channels());
    sndFile.readf(samples.data(), sndFile.frames()); // read all samples

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
            
            vector<vector<short>> channelsIn = separate(samples, sndFile.channels());
            vector<vector<short>> channelsOut(channelsIn.size());
            for(int i=0; i<channelsIn.size(); i++) {
                channelsOut[i] = echo.apply(channelsIn[i]); 
            }
            samplesOut = join(channelsOut);

            cout << samples.size() << '\n';
            cout << samplesOut.size() << '\n';

            sndFileOut.writef(samplesOut.data(), samplesOut.size()/sndFile.channels());
            return 0;
        }
    case d:
        {
            float delay = stof(argv[3]);
            if (delay < 0 || delay > 20000) {
                cerr << "Error: invalid delay value: " << delay << "\n";
                usage(argv);
                return 1;
            }

            Wav_Delay delayEffect = Wav_Delay{sndFile.samplerate(), delay};
            cout << "delay criado \n";
            
            vector<vector<short>> channelsIn = separate(samples, sndFile.channels());
            vector<vector<short>> channelsOut(channelsIn.size());
            for(int i=0; i<channelsIn.size(); i++) {
                channelsOut[i] = delayEffect.apply(channelsIn[i]); 
            }
            samplesOut = join(channelsOut);

            cout << samples.size() << '\n';
            cout << samplesOut.size() << '\n';

            sndFileOut.writef(samplesOut.data(), samplesOut.size()/sndFile.channels());
            return 0;
        }
    case 'A':

        break;
    default:
        usage(argv);
        return 1;
    }
    

    
    return 0;
}	


