#ifndef __SINEWAVEAUDIOCHUNK_H__
#define __SINEWAVEAUDIOCHUNK_H__

#include "AudioChunk.h"

class SineWaveAudioChunk : public AudioChunk {
	private : 
		double amplitude;
		double frequency;
		double phase;
	public :
		SineWaveAudioChunk(double samplingRate, double duration, double amplitude, double frequency, double phase);
		~SineWaveAudioChunk();
		double getamplitude();
		void setamplitude(double);
		double getfrequency();
		void setfrequency(double);
		double getphase();
		void setphase(double);
		void genrate_samples();
};
#endif // __SINEWAVEAUDIOCHUNK_H__
