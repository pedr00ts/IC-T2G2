#include "sndCodec.h"
sndCodec::sndCodec(uint32_t m, bool mode) {
    sndCodec::golomb = Golomb{m, mode};
}

sndCodec::sndCodec(bool mode) {
    sndCodec::golomb = Golomb{mode};
}

sndCodec::~sndCodec() {
}

void sndCodec::encode(SndfileHandle& sndFile, string encodedFilePath) {
    BitStream writeStream {encodedFilePath};
    uint32_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE);
    
    while(nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) {
        for (short frame : samples) {
            vector<bool> code = golomb.encode(frame);
            vector<bool> aux;
            while(code.size() >= 64) {
                aux = vector<bool>(code.begin(), code.begin() + 63);
                for (size_t i = 0; i < 64 && i < code.size(); i++) {
                    writeStream.writeNBits(aux);
                }
                code.erase(code.begin(), code.begin()+64);
            }

            for (bool b : code)
                writeStream.writeBit(b);
            }
    }

    writeStream.close();
}

void sndCodec::decode(string encodedFilePath, SndfileHandle& sndFile) {
    BitStream readStream {encodedFilePath};
    vector<bool> code;
    bool b;

    while(readStream.hasNext()) {                       // file loop
        while(readStream.hasNext()) {                   // prefix loop
            b = readStream.readBit();
            code.push_back(b);
            if (b)
                break;
        }

        while(read)

    }

}

