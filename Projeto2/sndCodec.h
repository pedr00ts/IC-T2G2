#include "golomb.h"
#include "bitstream.h"
#include <sndfile.hh>
#define FRAMES_BUFFER_SIZE 65536


#ifndef SNDCODEC
#define SNDCODEC
class sndCodec
{
private:
    Golomb golomb;
    vector<short> last_values;
    bool dynamic;
    float alfa(int n);
    uint32_t bestM(int n);
    short average(vector<short> const& values);

public:
    sndCodec(uint32_t m, bool mode);
    sndCodec(bool mode);
    sndCodec() = default;
    ~sndCodec() = default;

    void encode(SndfileHandle& sndFile, string encodedPath);
    void decode(BitStream& encodedPath, string decodedPath);
};
#endif
