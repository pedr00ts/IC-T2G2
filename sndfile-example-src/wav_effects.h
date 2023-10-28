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
        float mix;

    public:
        Wav_Delay (int sampleRate, float delay) {
            Wav_Delay::delay = delay;
            mix = 0.4;
        }

        vector<short> apply(vector<short>& in) {
            int delayInSamples = (int) delay*sampleRate;
            vector<short> out(in.size() + delayInSamples);

            int effect;
            for (int i=0; i < out.size(); i++) {
                if (i < delayInSamples) {
                    out[i] = in[i];
                } else {
                    out[i] = (1-mix) * in[i] + mix * in[i - delayInSamples];
                }
            }
            delayInSamples += delayInSamples;

            return out;
        }
};

class Wav_Echo: public Wav_Effect {
    private:
        float delay;
        float decay;
        float mix;

    public:
        Wav_Echo (int sampleRate, float delay, float decay) {
            Wav_Effect::sampleRate = sampleRate;
            Wav_Echo::delay = delay;
            Wav_Echo::decay = decay;
            mix = 0.4;
        }

        vector<short> apply(vector<short>& in) {
            int delayInSamples = (int) delay*sampleRate;
            vector<short> out(in.size() + delayInSamples);

            int effect;
            for (int i=0; i < out.size(); i++) {
                if (i < delayInSamples) {
                    out[i] = in[i];
                } else {
                    out[i] = (1-mix) * in[i] + mix * out[i - delayInSamples] * decay;
                }
            }
            delayInSamples += delayInSamples;

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