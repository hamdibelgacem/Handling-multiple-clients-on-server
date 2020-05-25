#ifndef __SILENTAUDIOCHUNK_H__
#define __SILENTAUDIOCHUNK_H__

#include "iostream"
#include "SilentAudioChunk.h"

using namespace std;

SilentAudioChunk::SilentAudioChunk(double samplingRate, double duration):
AudioChunk(samplingRate, duration)
{
	cout << "Create a new SilentAudioChunk : " << endl;
	this->samplesBuffer.resize(7);
	this->samplesBuffer[0] = 0x000E;
}

SilentAudioChunk::~SilentAudioChunk()
{
//	cout << "Destroy SilentAudioChunk" << endl;

}

void SilentAudioChunk::genrate_samples()
{
	//Write the length of the Audio chunk in the fisrt byte
	this->samplesBuffer[0] = 0x000E;

	//Write ehe ASCII character string "slnt" in the first 2 words = chunkid
	this->samplesBuffer[1] = 0x736c; //"sl"
	this->samplesBuffer[2] = 0x6e74; //"nt"

	this->samplesBuffer[3] = 0x0000;
	this->samplesBuffer[4] = 0x0004;
	
	this->samplesBuffer[5] = this->samplesNumber >> 16;
	this->samplesBuffer[6] = this->samplesNumber &  0x0000FFFF;
}

#endif // __SILENTAUDIOCHUNK_H__
