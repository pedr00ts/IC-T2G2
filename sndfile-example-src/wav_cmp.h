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

    size_t channels() {
        return nChannels;
    }

    bool validChannel(short channel) {
        return 0 <= channel < nChannels;
    }


    WavCmp(SndfileHandle& edited, SndfileHandle& original) {
        if (edited.channels() != original.channels()) {
            cerr << "Error - provided files have incompatible number of channels\n";
            return;
        }
        nChannels = original.channels();

        // debug
        // cout << "nChannels: " << nChannels << "\n";

        // Read all frames from files to samples
        size_t i { 0 };   
        // original file -> samplesA
        samplesA = vector<short>(original.frames());    
        while(original.readf(&samplesA[i++], FRAMES_BUFFER_SIZE));
        i = 0;  // reset index
        // edited file -> samplesB
        samplesB = vector<short>(edited.frames());    
        while(edited.readf(&samplesB[i++], FRAMES_BUFFER_SIZE));

        if (samplesA.size() != samplesB.size()) {
            cerr << "Error - provided files have incompatible amount of samples\n";
            return;
        }

        // debug
        // cout << "SamplesA size: " << samplesA.size() << "\n";
        // cout << "SamplesB size: " << samplesB.size() << "\n";
        // for(int i=0; i<10; i++) {
        //     cout << "[" << i << "]: " << samplesA[i] << " " << samplesB[i] << "\n";
        // }

    }

    // return meanSquaredError between samplesA and samplesB for specified channel
    double meanSquaredError (short channel) {
        if(!validChannel(channel))
            throw(channel);
            
        double res { };
        size_t i = channel;
        while (i < samplesA.size()) {
            res += pow(abs(samplesA[i] - samplesB[i]), 2);
            i += nChannels;
        }
        
        return res/(samplesA.size()/nChannels);
    }

    // return meanSquaredError between samplesA and samplesB for average of channels
    double meanSquaredErrorAvg () {
        double res { };
        double sumA { };
        double sumB { };
         
        for (size_t i = 1; i <= samplesA.size(); i++) {
            sumA += samplesA[i-1];
            sumB += samplesB[i-1];

            // i == nChannels-1 -> calculate MSE between A and B samples and reset sums  
            if (i % (nChannels) == 0) {
                res += pow(abs((sumA/nChannels)-(sumB/nChannels)), 2);
                sumA = 0;
                sumB = 0;
            }
        }

        return res/(samplesA.size()/nChannels); // size / nChannels == nElements ()
    }


    double maxAbsoluteError(short channel) {
        if(!validChannel(channel))
            throw(channel);

        double max {};
        
        size_t i = channel;
        
        while (i < samplesA.size()) {
            if ((abs(samplesA[i] - samplesB[i])) > max)
                max = (abs(samplesA[i] - samplesB[i]));
            i += nChannels;
        }

        return max;
    }


    double maxAbsoluteErrorAvg () {
        double max { };
        double sumA { };
        double sumB { };
         
        for (size_t i = 1; i <= samplesA.size(); i++) {
            sumA += samplesA[i-1];
            sumB += samplesB[i-1];

            if (i % (nChannels) == 0) {
                if ((abs(sumA/nChannels - sumB/nChannels)) > max)
                    max = (abs(sumA/nChannels - sumB/nChannels));
                sumA = 0;
                sumB = 0;
            }
        }

        return max; // size / nChannels == nElements ()
    }


    double SNR (short channel) {
        if(!validChannel(channel))
            throw(channel);

        size_t S {};

        for (size_t i = channel; i < samplesA.size(); i+=nChannels) {
            S += pow(samplesA[i], 2);
        }

        S /= (samplesA.size() / nChannels); 
        return S/meanSquaredError(channel);
    }


    double SNR_Avg () {
        double S { };
        double sumA { };
         
        for (size_t i = 1; i <= samplesA.size(); i++) {
            sumA += samplesA[i-1];

            if (i % (nChannels) == 0) {
                S += pow(sumA/nChannels, 2);
                sumA = 0;
            }
        }
        S /= (samplesA.size()/nChannels);

        return S/meanSquaredErrorAvg(); // size / nChannels == nElements ()
    }






    // print results
    void print () {

    }

    


};
