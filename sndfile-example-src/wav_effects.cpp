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

int main(int argc, char *argv[]) {

    // verificar argumentos
    if(argc != 6) {
            cerr << "Error: wrong number of args\n";
            usage(argv);
            return 1;
    }

    // Verificar ficheiros de entrada e saída
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

    // Ler ficheiro de entrada
	// vector<short> samples(sndFile.frames() * sndFile.channels());
    // sndFile.readf(samples.data(), sndFile.frames()); // read all samples
    // vector<short> samplesOut();

    vector<short> samplesIn(FRAMES_BUFFER_SIZE * sndFile.channels());
    switch(argv[2][1]) {
    case e:
        {
            // Verificar parametros
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

            // Criar efeito
            Wav_Echo echo = Wav_Echo{sndFile.samplerate(), sndFile.channels(), delay, decay};
            cout << "echo criado \n";
            
            // aplicar efeito (ficheiro todo)
            // vector<vector<short>> channelsIn = separate(samples, sndFile.channels());
            // vector<vector<short>> channelsOut(channelsIn.size());
            // for(int i=0; i<channelsIn.size(); i++) {
            //     channelsOut[i] = echo.apply(channelsIn[i]); 
            // }
            // samplesOut = join(channelsOut);
            // sndFileOut.writef(samplesOut.data(), samplesOut.size()/sndFile.channels());

            // aplicar efeito em blocos
            size_t nFrames;
            while(nFrames = sndFile.readf(samplesIn.data(), FRAMES_BUFFER_SIZE)) {
                // ler bloco
                samplesIn.resize(nFrames * sndFile.channels());
                // aplicar efeito e escrever bloco
                sndFileOut.writef(echo.apply(samplesIn).data(), nFrames);
            }

            cout << samplesIn.size() << '\n';

            return 0;
        }
    case d:
        {
            // float delay = stof(argv[3]);
            // if (delay < 0 || delay > 20000) {
            //     cerr << "Error: invalid delay value: " << delay << "\n";
            //     usage(argv);
            //     return 1;
            // }

            // Wav_Delay delayEffect = Wav_Delay{sndFile.samplerate(), delay};
            // cout << "delay criado \n";
            
            // vector<vector<short>> channelsIn = separate(samplesIn, sndFile.channels());
            // vector<vector<short>> channelsOut(channelsIn.size());
            // for(int i=0; i<channelsIn.size(); i++) {
            //     channelsOut[i] = delayEffect.apply(channelsIn[i]); 
            // }
            // samplesOut = join(channelsOut);

            // cout << samplesIn.size() << '\n';
            // cout << samplesOut.size() << '\n';

            // sndFileOut.writef(samplesOut.data(), samplesOut.size()/sndFile.channels());
            // return 0;
        }
    case 'A':

        break;
    default:
        usage(argv);
        return 1;
    }
    

    
    return 0;
}	


