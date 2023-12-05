#include "sndCodec.h"

sndCodec::sndCodec(uint32_t m, bool mode) {
    sndCodec::golomb = {m, mode};
    sndCodec::last_values = {};
}

sndCodec::sndCodec(bool mode) {
    sndCodec::golomb = {mode};
    sndCodec::last_values = {};
}

// Predictive Golomb Encoder
void sndCodec::encode(SndfileHandle& sndFile, string encodedPath) {
    BitStream stream(encodedPath);                 // create code stream

    // encode header
    golomb.encodeNext(stream, sndFile.format());
    golomb.encodeNext(stream, sndFile.channels());
    golomb.encodeNext(stream, sndFile.samplerate());
    cout << "Encoding snd file with format=" << sndFile.format() << ", channels=" << sndFile.channels() << ", samplerate=" << sndFile.samplerate() << endl;  // DEBUG
    // encode data
    uint32_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());         // frames buffer
    int res;                                                                // residual to encode
    while(nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) {    // read all frames (in blocks of frames buffer size)
        // cout << nFrames << " frames read" << endl;      // DEBUG
        samples.resize(nFrames * sndFile.channels());
        for (short frame : samples) {                   // for frames from the buffer
            //cout << frame << '\n';                    // DEBUG
            switch(last_values.size()) {                // obtain frame residual
                case 0:
                    res = frame;
                    break;
                case 1:
                    res = frame - last_values[0];
                    break;
                case 2:
                    res = frame - 2*last_values[1] + last_values[0];
                    break;
                default:
                    res =  frame - 3*last_values[2] + 3*last_values[1] - last_values[0];
                    last_values.erase(last_values.begin());
            }
            last_values.push_back(frame);
            golomb.encodeNext(stream, res);                // encode frame residual           
        }
    }
    stream.close();                                        // close code stream
}

// Predictive Golomb Decoder
void sndCodec::decode(BitStream& stream, string decodedPath) {
    // decode header
    int header[3];          // format, channels, samplerate
    for(int param=0; param < 3 && golomb.decodeNext(stream, header[param]); param++);
    // create sndFile from header
    cout << "Decoding snd file with format=" << header[0] << ", channels=" << header[1] << ", samplerate=" << header[2] << endl;  // DEBUG
    SndfileHandle sndFile {decodedPath, SFM_WRITE, header[0], header[1], header[2]};
    // decode data
    vector<short> samples;             // decoded frames buffer
    short frame;                       // decoded frame
    int res;                           // decoded residual
    while (golomb.decodeNext(stream, res)) {    // decode all residuals from the code stream     
        switch(last_values.size()) {            // obtain frame from decoded residual
            case 0:
                frame = (short)res;
                break;
            case 1:
                frame = (short)res + last_values[0];
                break;
            case 2:
                frame = (short)res + 2*last_values[1] - last_values[0];
                break;
            default:
                frame = (short)res + 3*last_values[2] - 3*last_values[1] + last_values[0];
                last_values.erase(last_values.begin());
        }
        last_values.push_back(frame);
        samples.push_back(frame);                // update frames buffer
        //cout << frame << '\n';                 // DEBUG
        if(samples.size() == FRAMES_BUFFER_SIZE * sndFile.channels()) {
            // cout << "Writing frames buffer" << endl;                    // DEBUG
            sndFile.writef(samples.data(), FRAMES_BUFFER_SIZE);         // write frames buffer
            samples.resize(0);                                          // reset frames buffer
        }
    }
    sndFile.writef(samples.data(), samples.size()/sndFile.channels());      // write rem frames in frames buffer
    stream.close();                                                         // close code stream
}
