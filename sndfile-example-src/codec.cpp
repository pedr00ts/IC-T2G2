#define FRAMES_BUFFER_SIZE 65536

#include "codec.h"
#include <sndfile.hh>


using namespace std;

void usage(char* argv[]) {
    cerr << "Usage: " << argv[0] << " <-c|-d> <nBits> <inputFile> <outputFile>\n";
}

void help(char* argv[]) {
    usage(argv);
    cout << "Operations: compress, decompress\n";
    cout << "0 < nBits < 16\n";
}

void compress(SndfileHandle audioFile, fstream& compFile, size_t bitsPerSample) {
    Codec c{};
    BitStream bs{};
    size_t nFrames{};
    vector<short> samples(FRAMES_BUFFER_SIZE); 
    vector<bool> cmpBits;

    compFile.seekp(compFile.beg);
    audioFile.seek(0, 0);
    
    while((nFrames = audioFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames);
		cmpBits = c.compress(samples, bitsPerSample);
        for (size_t i = 0; i < cmpBits.size(); i++) {
            bs.writeBit(compFile, cmpBits[i], i%8);
        }
	}

    compFile.close();
}

void decompress(fstream& compFile, SndfileHandle audioFile, size_t bitsPerSample) {
    Codec c{};
    BitStream bs{};
    size_t nFrames{};
    vector<short> values(FRAMES_BUFFER_SIZE);
    vector<bool> bits(64);

    compFile.seekg(compFile.end);
    size_t size = compFile.tellg();
    compFile.seekg(compFile.beg);
    audioFile.seek(0, 0);

    while(size > 64) {
        bits = bs.readNBits(compFile, 64, 0);
        values = c.decompress(bits, bitsPerSample);
        audioFile.writef(values.data(), 4);
        size -= 64;
    }

    bits = bs.readNBits(compFile, size, 0);
    values = c.decompress(bits, bitsPerSample);
    audioFile.writef(values.data(), size/16 + (size%16 != 0));
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        usage(argv);
        return 1;
    }

    SndfileHandle sndFile;
    fstream compFile;
    bool comp;

    if (argv[1][1] == 'c') {
        sndFile = SndfileHandle{argv[3]};
        compFile = fstream{argv[4]};
        comp = 1;
    } else if(argv[1][1] == 'd') {
        SndfileHandle sndFile = SndfileHandle(argv[4], SFM_WRITE, SF_FORMAT_WAV, 1, 44100);
        compFile = fstream{argv[3]};
    } else {
        cerr << "Error: invalid operation\n";
        help(argv);
        return 1;
    }
	
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

    if ((stoi(argv[2]) < 0) || (stoi(argv[2]) > 16)) {
        help(argv);
        return 1;
    }

    if (comp) {
        compress(sndFile, compFile, stoi(argv[2]));
    } else {
        decompress(compFile, sndFile, stoi(argv[2]));
    }


}