#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_cmp.h"

using namespace std;

void usage(char *argv[]) {
	cerr << "Usage: " << argv[0] << " <edited file> <orignal file>\n";
}

int validateSndFile(const SndfileHandle& sndFile) {
  if(sndFile.error()) {
		cerr << "Error: " << "file invalid\n";
		return 0;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 0;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 0;
	}

  return true;
}

void showLsquaredNorm(WavCmp& comp) {
  cout << ">> L2 NORM\n";
  for (size_t i=0; i < comp.channels(); i++) {
    cout << "-- Channel " << i << ": " << comp.meanSquaredError(i) << "\n";
  }
  cout << "-- Avg: " << comp.meanSquaredErrorAvg() << "\n";
  cout << "\n";
}

void showLinfinityNorm(WavCmp& comp) {
  cout << ">> L\u221e NORM\n";
  for (size_t i=0; i < comp.channels(); i++) {
    cout << "-- Channel " << i << ": " << comp.maxAbsoluteError(i) << "\n";
  }
  cout << "-- Avg: " << comp.maxAbsoluteErrorAvg() << "\n";
  cout << "\n";
}

void showSNR(WavCmp& comp) {
  cout << ">> SNR\n";
  for (size_t i=0; i < comp.channels(); i++) {
    cout << "-- Channel " << i << ": " << comp.SNR(i) << "\n";
  }
  cout << "-- Avg: " << comp.SNR_Avg() << "\n";
  cout << "\n";
}


int main(int argc, char *argv[]) {

	if(argc != 3) {
		cerr << "Error: wrong number of args\n";
		usage(argv);
		return 1;
	}

	SndfileHandle edited { argv[1] };
  if (!validateSndFile(edited))
    return 0;
  SndfileHandle original { argv[2] };
  if (!validateSndFile(original))
    return 0;
	
  WavCmp comp {edited, original};

  showLsquaredNorm(comp);
  showLinfinityNorm(comp);
  showSNR(comp);

  return 0;
}