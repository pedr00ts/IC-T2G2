#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>
#define  FRAMES_BUFFER_SIZE 65536

using namespace std;

class Wav_Effect {
    public:
        virtual vector<short> apply(vector<short>& in) {return in;}
};

class Wav_Delay: public Wav_Effect {
    private:
        float time;
        int feedback;

    public:
        Wav_Delay (float time, int feedback) {
            Wav_Delay::time = time;
            Wav_Delay::feedback = feedback;
        }

        vector<short> apply(vector<short>& in) {
            vector<short> out = out;
            return out;
        }
};

class Wav_Echo: public Wav_Effect {
    private:
        float delay;
        float decay;

    public:
        Wav_Echo (float delay, float decay) {
            Wav_Echo::delay = delay;
            Wav_Echo::decay = decay;
        }

        vector<short> apply(vector<short>& in) {
            vector<short> out = in;
            return out;
        }
};

class Wav_Reverb: public Wav_Effect {
    private:
        float delay;
        float decay;

    public:
        Wav_Reverb (float delay, float decay) {
            Wav_Reverb::delay = delay;
            Wav_Reverb::decay = decay;
        }

        vector<short> apply(vector<short>& in) {
            vector<short> out = in;
            return out;
        }
};