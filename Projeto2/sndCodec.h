#include "golomb.h"
#include "bitstream.h"
#include <sndfile.hh>
#define FRAMES_BUFFER_SIZE 65536

class sndCodec
{
private:
    Golomb golomb;

public:
    sndCodec(uint32_t m, bool mode);
    sndCodec(bool mode);
    ~sndCodec();

    void encode(SndfileHandle& sndFile, string encodedFilePath);
    void decode(string encodedFilePath, SndfileHandle& sndFile);
};


