#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#define  FRAMES_BUFFER_SIZE 65536

// meanSquaredError - each channel and avg
// maxAbsError      - ...
// sigNoiseratio    - ...
// implement for each function channel and avg versions?

using namespace std;

class WavCmp {

    private:
        SndfileHandle fileA;
        SndfileHandle fileB;
        vector <short> samplesA;
        vector <short> samplesB;  

    public:


    WavCmp(const SndfileHandle& file1, const SndfileHandle& file2) {
        if (file1.channels() != file2.channels()) {
            cerr << "Error - provided files have incompatible number of channels\n";
            return;
        }
        fileA = file1;
        fileB = file2;

        // Read all frames from fileA to samplesA
        size_t totalFrames;
        size_t nFrames {} ;       
        do {
            samplesA.reserve(fileA.channels() * FRAMES_BUFFER_SIZE);
            totalFrames += nFrames;
        } while((nFrames = fileA.readf((short*)samplesA.back(), FRAMES_BUFFER_SIZE)));

        // Read all frames from fileB to samplesB
        totalFrames = 0;
        nFrames = 0;
        do {
            samplesB.reserve(fileB.channels() * FRAMES_BUFFER_SIZE);
            totalFrames += nFrames;
        } while((nFrames = fileA.readf((short*)samplesA.back(), FRAMES_BUFFER_SIZE)));

        if (samplesA.size() != samplesB.size()) {
            cerr << "Error - provided files have incompatible sizes\n";
            return;
        }

    }

    //




    double meanSquaredError (short channel) {
        double result { };



    }






    // print results
    void print () {

    }

    


};
