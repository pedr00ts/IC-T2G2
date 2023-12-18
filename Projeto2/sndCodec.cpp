#include "sndCodec.h"

// Fixed module codec constructor
sndCodec::sndCodec(uint32_t m, bool mode) {
    sndCodec::golomb = {m, mode};
    sndCodec::last_values = {};
    sndCodec::dynamic = false;
}

// Variable module codec constructor
sndCodec::sndCodec(bool mode) {
    sndCodec::golomb = {4, mode};
    sndCodec::last_values = {};
    sndCodec::dynamic = true;
}

// Returns alfa for a distribution of type: alfa^i * (1-alfa)
float sndCodec::alfa(int n) {
    cout << "Finding the alfa for " << n << endl;
    float res = 0;
    int an = abs(n); 
    if (an > 0)
        res = (float)an/(float)(an + 1);
    cout << "alfa=" << res << endl;
    return res;
}

// Returns the best m to encode a given integer n
uint32_t sndCodec::bestM(int n) {
    cout << "Finding the best m for " << n << endl;
    uint32_t res = 1;
    float a = alfa(n);
    if (a > 0)
        res = ceil(-1/log2(a));
    cout << "m=" << res << endl;
    return res;
}

short sndCodec::average(vector<short> const& values) {
    cout << "Calculating the average" << endl;
    if (values.empty())
        return 0;
    int sum = 0;
    for (auto v: values)
        sum += v;
    return (short) (sum / values.size());
}


// Predictive Golomb Encoder
void sndCodec::encode(SndfileHandle& sndFile, string encodedPath) {
    BitStream stream(encodedPath);                 // create code stream

    // encode header
    if (dynamic)
        golomb.setM(bestM(sndFile.format()));
    golomb.encodeNext(stream, sndFile.format());
    if (dynamic)
        golomb.setM(bestM(sndFile.channels()));
    golomb.encodeNext(stream, sndFile.channels());
    if (dynamic)
        golomb.setM(bestM(sndFile.samplerate()));
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
            if (dynamic)
                golomb.setM(bestM(res));
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
