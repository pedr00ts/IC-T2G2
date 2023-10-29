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
     e = 'e', d = 'd', r = 'r'   
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
            // Verificar parametros
            float delay = stof(argv[3]);
            if (delay < 0 || delay > 20000) {
                cerr << "Error: invalid delay value: " << delay << "\n";
                usage(argv);
                return 1;
            }
            cout << delay << '\n';

            // Criar efeito
            Wav_Delay delayEffect = Wav_Delay{sndFile.samplerate(), sndFile.channels(), delay};
            cout << "delay criado \n";

            // aplicar efeito em blocos
            size_t nFrames;
            while(nFrames = sndFile.readf(samplesIn.data(), FRAMES_BUFFER_SIZE)) {
                // ler bloco
                samplesIn.resize(nFrames * sndFile.channels());
                // aplicar efeito e escrever bloco
                sndFileOut.writef(delayEffect.apply(samplesIn).data(), nFrames);
            }

            cout << samplesIn.size() << '\n';

            return 0;
        }
    case 'r':
        // Verificar parametros
            float maxDelay = stof(argv[3]);
            float maxDecay = stof(argv[4]);
            if (maxDelay < 0 || maxDelay > 20000) {
                cerr << "Error: invalid delay value: " << maxDelay << "\n";
                usage(argv);
                return 1;
            }
            if (maxDecay < 0 | maxDecay >= 1) {
                cerr << "Error: invalid decay value: " << maxDecay << "\n";
                usage(argv);
                return 1;
            }
            cout << maxDelay << '\n';
            cout << maxDecay << '\n';

            
    }
    

    
    return 0;
}	


