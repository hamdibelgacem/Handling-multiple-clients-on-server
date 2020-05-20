#ifndef __SILENTWAVEAUDIOCHUNK_H__
#define __SILENTWAVEAUDIOCHUNK_H__

#include <cstdint>
#include <cstring>
#include <sstream>
#include "AudioChunk.h"

class SilentAudioChunk : public AudioChunk {
public :
	SilentAudioChunk(uint32_t samplingRate, uint32_t duration);
	~SilentAudioChunk();
	void genrate_samples();
    void printSilent();
};
#endif // __SILENTWAVEAUDIOCHUNK_H__
