#include "sndCodec.h"

void readSamples(string sndPath) {
    SndfileHandle sndFile {sndPath};
    // print header
    cout << sndFile.format() << endl;       // format
    cout << sndFile.channels() << endl;     // channels
    cout << sndFile.samplerate() << endl;   // srate
    // print data
    uint32_t nFrames;
    vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
    while(nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE)) {
        samples.resize(nFrames * sndFile.channels());
        for (short frame : samples)
            cout << frame << endl;
    }
}

void writeSamples(string samplesPath, string sndPath) {
    ifstream stream(samplesPath);
    SndfileHandle sndFile;
    string line;
    // decode header
    int header[3];
    int ih = 0;
    while(getline(stream, line) && ih < 3) {    // header vaues to read     
        header[ih] = stoi(line);    // read header values
        ih++;                       // increment header index
    }                                           // end of header reached
    // create sndFile from header
    sndFile = {sndPath, SFM_WRITE, header[0], header[1], header[2]};
    cout << header[0] << ' ' << header[1] << ' ' << header[2] << endl;  // DEBUG
    // decode data
    vector<short> samples;
    short frame;
    while (getline(stream, line)) {
        frame = (short) stoi(line);         // read data frame
        samples.push_back(frame);           // update frames buffer 
        // cout << samples[ifr] << endl;  // DEBUG
        if(samples.size() == FRAMES_BUFFER_SIZE * sndFile.channels()) {          // frames buffer size reached
            cout << "Writing buffer with " << samples.size() << " frames..." << endl;  // DEBUG
            sndFile.writef(samples.data(), FRAMES_BUFFER_SIZE);     // write frames buffer
            samples.resize(0);                                  // reset frames buffer
        }
    }
    sndFile.writef(samples.data(), samples.size()/sndFile.channels());             // write rem frames in frames buffer
    stream.close();
}

void usage(char* argv[]) {
    cerr << "Usage: " << argv[0] << " <op> <params>" << endl;
    cerr << "	List of operations:" << endl;
	cerr << "	-r      read <sndFile>." << endl;
	cerr << "	-w      write <samplesFile> to <sndFile>." << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error! Wrong number of args." << endl;
        usage(argv);
        return 1;
    }

    string sndPath, samplesPath;
    switch(argv[1][1]) {
        case 'r':
            if (argc != 3) {
                cerr << "Error! Wrong number of args - missing sndFile path" << endl;
                usage(argv);
                return 1;
            }
            sndPath = argv[2];

            readSamples(sndPath);
            break;
        case 'w':
            if (argc < 3) {
                cerr << "Error! Wrong number of args - missing samplesFile path" << endl;
                usage(argv);
                return 1;
            }
            samplesPath = argv[2];

            if (argc != 4) {
                cerr << "Error! Wrong number of args - missing sndFile path" << endl;
                usage(argv);
                return 1;
            }
            sndPath = argv[3];

            writeSamples(samplesPath, sndPath);
            break;
        default:
            cerr << "invalid operation\n";
    }
    
    return 0;
}