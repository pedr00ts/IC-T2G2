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
        vector <short> samplesA;
        vector <short> samplesB;
        size_t nChannels; 

    public:


    WavCmp(SndfileHandle& fileA, SndfileHandle& fileB) {
        if (fileA.channels() != fileA.channels()) {
            cerr << "Error - provided files have incompatible number of channels\n";
            return;
        }
        nChannels = fileA.channels();

        // Read all frames from fileA to samplesA
        size_t nFrames {} ;       
        do {
            samplesA.reserve(fileA.channels() * FRAMES_BUFFER_SIZE);
        } while((nFrames = fileA.readf((short*)samplesA.back(), FRAMES_BUFFER_SIZE)));

        // Read all frames from fileB to samplesB
        nFrames = 0;
        do {
            samplesB.reserve(fileB.channels() * FRAMES_BUFFER_SIZE);
        } while((nFrames = fileA.readf((short*)samplesA.back(), FRAMES_BUFFER_SIZE))); // nframes > 0 --> loop

        if (samplesA.size() != samplesB.size()) {
            cerr << "Error - provided files have incompatible amount of samples\n";
            return;
        }

    }

    //



    // return meanSquaredError between samplesA and samplesB for specified channel
    double meanSquaredError (short channel) {
        double res { };
        size_t i = channel;
        
        while (i < samplesA.size()) {
            res += abs(pow(samplesA[i], 2) - pow(samplesB[i], 2));
            i += nChannels;
        }
        
        return res/nChannels;
    }

    // return meanSquaredError between samplesA and samplesB for average of channels
    double meanSquaredErrorAvg () {
        double res { };
        double sumA { };
        double sumB { };
         
        for (size_t i = 0; i < samplesA.size(); i = (i+1) % nChannels) {
            sumA += samplesA[i];
            sumB += samplesB[i];

            // i == nChannels-1 -> calculate MSE between A and B samples and reset sums  
            if (i == (nChannels - 1)) {
                res += abs(pow((sumA/nChannels), 2) - pow((sumA/nChannels), 2));
                sumA = 0;
                sumB = 0;
            }
        }

        return res/(samplesA.size()/nChannels); // size / nChannels == nElements ()
    }






    // print results
    void print () {

    }

    


};
