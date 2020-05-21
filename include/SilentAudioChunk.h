#ifndef __SILENTWAVEAUDIOCHUNK_H__
#define __SILENTWAVEAUDIOCHUNK_H__

#include <cstdint>
#include <cstring>
#include <sstream>
#include "AudioChunk.h"

class SilentAudioChunk : public AudioChunk {
public :
	SilentAudioChunk(double samplingRate, double duration);
	~SilentAudioChunk();
	void genrate_samples();
};
#endif // __SILENTWAVEAUDIOCHUNK_H__
