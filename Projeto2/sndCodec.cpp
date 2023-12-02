#include "sndCodec.h"

sndCodec::sndCodec(uint32_t m, bool mode) {
    sndCodec::golomb = {m, mode};
}

sndCodec::sndCodec(bool mode) {
    sndCodec::golomb = {mode};
}

void sndCodec::encode(SndfileHandle& sndFile, string encodedPath) {
    GolombStream gstream(golomb, encodedPath);
    uint32_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    while(nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) {
        samples.resize(nFrames * sndFile.channels());
        for (short frame : samples)
            gstream.encodeNext(frame);
    }
    gstream.close();
}

void sndCodec::decode(string encodePath, string decodePath) {
    GolombStream gstream(golomb, encodePath);
    
    // decode header
    int samplerate = 44100;
    int channels = 2;
    // create SndFileHandle
    SndfileHandle sndFile {decodePath, SFM_WRITE, 0, channels, samplerate};
    // decode data
    vector<short> samples {};
    while (gstream.hasNext()){
        samples.push_back(gstream.decodeNext());
        if(samples.size() == FRAMES_BUFFER_SIZE) {
            sndFile.writef(samples.data(), samples.size());
            samples.clear();
        }
    }
    sndFile.writef(samples.data(), samples.size());
}

