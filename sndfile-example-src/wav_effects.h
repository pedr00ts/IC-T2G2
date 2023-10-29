#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <cmath>

using namespace std;

class Wav_Effect {
    protected:
        int sampleRate;
        // buffer
        int nChannels;
        vector<vector<short>> channels;
        int p;
        
    public:
        virtual vector<short> apply(vector<short>& in) {return in;}
};

class Wav_Delay: public Wav_Effect {
    private:
        float delay;
        float mix;
        // aux
        int delayInSamples;

    public:
        Wav_Delay (int sampleRate, int nChannels, float delay) {
            delayInSamples = (int) (delay*sampleRate);
            Wav_Effect::nChannels = nChannels;
            Wav_Effect::channels = vector<vector<short>>(nChannels, vector<short>(delayInSamples));
            p = 0;

            Wav_Effect::sampleRate = sampleRate;
            Wav_Delay::delay = delay;
            mix = 0.4;
        }

        vector<short> apply(vector<short>& samplesIn) {
            vector<short> out(samplesIn.size());
            cout << delayInSamples << "\n";
            
            // aplicar efeito em cada canal
            for (int i=0; i < samplesIn.size(); i+=nChannels) {
                for (int c=0; c < nChannels; c++) {
                    if (p < delayInSamples) {
                        channels[c][p] = samplesIn[i+c];
                    } else {
                        out[i+c] = (1-mix) * samplesIn[i+c] + mix * channels[c][p % delayInSamples];
                        channels[c][p % delayInSamples] = samplesIn[i+c];
                    }
                }
                // atualizar indice global
                p++;
            }

            return out;
        }


};

class Wav_Echo: public Wav_Effect {
    private:
        float delay;
        float decay;
        float mix;
        // aux
        int delayInSamples;
        

    public:
        Wav_Echo (int sampleRate, int nChannels, float delay, float decay) {
            delayInSamples = (int) (delay*sampleRate);
            Wav_Effect::nChannels = nChannels;
            Wav_Effect::channels = vector<vector<short>>(nChannels, vector<short>(delayInSamples));
            p = 0;

            Wav_Effect::sampleRate = sampleRate;
            Wav_Echo::delay = delay;
            Wav_Echo::decay = decay;
            mix = 0.4;
        }

        vector<short> apply(vector<short>& samplesIn) {
            vector<short> out(samplesIn.size());
            cout << delayInSamples << "\n";
            
            // aplicar efeito em cada canal
            for (int i=0; i < samplesIn.size(); i+=nChannels) {
                for (int c=0; c < nChannels; c++) {
                    if (p < delayInSamples) {
                        channels[c][p] = samplesIn[i+c];
                    } else {
                        channels[c][p % delayInSamples] = (1-mix) * samplesIn[i+c] + mix * channels[c][p % delayInSamples] * decay;
                    }
                    out[i+c] = channels[c][p % delayInSamples];
                }
                // atualizar indice global
                p++;
            }

            return out;
        }
};

class Wav_Reverb: public Wav_Effect {
    private:
        float maxDelay;
        float maxDecay;
        vector<Wav_Echo> echoes;

    public:
        Wav_Reverb (int sampleRate, int nChannels, float maxDelay, float maxDecay) {
            Wav_Effect::sampleRate = sampleRate;
            Wav_Reverb::maxDelay = maxDelay;
            Wav_Reverb::maxDecay = maxDecay;
        }

        vector<short> apply(vector<short>& samplesIn) {
            vector<short> out = echoes[0].apply(samplesIn);
            for (Wav_Echo echo: echoes) {
                out = echo.apply(out);
            }
            return out;
        }
};