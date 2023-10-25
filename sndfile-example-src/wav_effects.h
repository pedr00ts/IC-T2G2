#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>

using namespace std;

class Wav_Effect {
    protected:
        int sampleRate;
    public:
        virtual vector<short> apply(vector<short>& in) {return in;}
};

class Wav_Delay: public Wav_Effect {
    private:
        float delay;
        int feedback;

    public:
        Wav_Delay (float delay, int feedback) {
            Wav_Delay::delay = delay;
            Wav_Delay::feedback = feedback;
        }

        vector<short> apply(vector<short>& in) {
            int delayInSamples = (int) (delay / sampleRate);
            vector<short> out(in.size() + delayInSamples);

            for (int i=0; i < out.size(); i++) {
                if (i < delayInSamples) {
                    out[i] = in[i];
                } else {
                    out[i] = in[i];
                }
            }

            return out;
        }
};

class Wav_Echo: public Wav_Effect {
    private:
        float delay;
        float decay;

    public:
        Wav_Echo (int sampleRate, float delay, float decay) {
            Wav_Effect::sampleRate = sampleRate;
            Wav_Echo::delay = delay;
            Wav_Echo::decay = decay;
        }

        vector<short> apply(vector<short>& in) {
            int delayInSamples = delay*sampleRate;
            vector<short> out(in.size() + delayInSamples);

            for (int i=0; i < out.size(); i++) {
                if (i < delayInSamples) {
                    out[i] = in[i];
                } else {
                    out[i] = in[i] + in[i - delayInSamples] * decay;
                }
            }

            return out;
        }
};

class Wav_Reverb: public Wav_Effect {
    private:
        float delay;
        float decay;

    public:
        Wav_Reverb (int sampleRate, float delay, float decay) {
            Wav_Effect::sampleRate = sampleRate;
            Wav_Reverb::delay = delay;
            Wav_Reverb::decay = decay;
        }

        vector<short> apply(vector<short>& in) {
            vector<short> out = in;
            return out;
        }
};