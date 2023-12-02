#include "sndCodec.h"

sndCodec::sndCodec(uint32_t m, bool mode) {
    sndCodec::golomb = {m, mode};
    sndCodec::last_values = {};
}

sndCodec::sndCodec(bool mode) {
    sndCodec::golomb = {mode};
    sndCodec::last_values = {};

}

void sndCodec::encode(SndfileHandle& sndFile, string encodedPath) {
    GolombStream gstream(golomb, encodedPath);
    uint32_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    while(nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) {
        samples.resize(nFrames * sndFile.channels());
        for (short frame : samples)
            switch(last_values.size()) {
                case 0:
                    gstream.encodeNext(frame);
                    break;
                case 1:
                    gstream.encodeNext(frame-last_values[0]);
                    break;
                case 2:
                    gstream.encodeNext(frame - 2*last_values[1] + last_values[0]);
                    break;
                default:
                    gstream.encodeNext(frame - 3*last_values[2] + 3*last_values[1] - last_values[0]);
                    last_values.erase(last_values.begin());
                last_values.push_back(frame);                
            }
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
    short res;
    while (gstream.hasNext()){
        res = gstream.decodeNext();
        short frame;
        switch(last_values.size()) {
            case 0:
                frame = res;
                break;
            case 1:
                frame = res + last_values[0];
                break;
            case 2:
                frame = res + 2*last_values[1] - last_values[0];
                break;
            default:
                frame = res + 3*last_values[2] - 3*last_values[1] + last_values[0];
                last_values.erase(last_values.begin());
        }
        last_values.push_back(frame);
        cout << frame << '\n';                  // DEBUG
        samples.push_back(frame);
        if(samples.size() == FRAMES_BUFFER_SIZE) {
            cout << "write buffer\n";
            sndFile.writef(samples.data(), samples.size());
            samples.clear();
        }
    }
    sndFile.writef(samples.data(), samples.size());
}

