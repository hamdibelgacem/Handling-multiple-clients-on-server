#ifndef __SINEWAVEAUDIOCHUNK_H__
#define __SINEWAVEAUDIOCHUNK_H__

#include "AudioChunk.h"

class SineWaveAudioChunk : public AudioChunk {
	private : 
		double amplitude;
		uint32_t frequency;
		double phase;
	public :
		SineWaveAudioChunk(double samplingRate, double duration, double amplitude, uint32_t frequency, double phase);
		~SineWaveAudioChunk();
		double getamplitude();
		void setamplitude(double);
		uint32_t getfrequency();
		void setfrequency(uint32_t);
		double getphase();
		void setphase(double);
		void genrate_samples();
};
#endif // __SINEWAVEAUDIOCHUNK_H__
