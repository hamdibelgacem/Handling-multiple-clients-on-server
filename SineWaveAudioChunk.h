#ifndef __SINEWAVEAUDIOCHUNK_H__
#define __SINEWAVEAUDIOCHUNK_H__

#include "AudioChunk.h"

class SineWaveAudioChunk : public AudioChunk {
	private : 
		uint32_t amplitude;
		uint32_t frequency;
		uint32_t phase;
	public :
		SineWaveAudioChunk(uint32_t samplingRate, uint32_t duration, uint32_t amplitude, uint32_t frequency, uint32_t phase);
		~SineWaveAudioChunk();
		uint32_t getamplitude();
		void setamplitude(uint32_t);
		uint32_t getfrequency();
		void setfrequency(uint32_t);
		uint32_t getphase();
		void setphase(uint32_t);
		void genrate_samples();
};
#endif // __SINEWAVEAUDIOCHUNK_H__
