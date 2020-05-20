#ifndef __SILENTAUDIOCHUNK_H__
#define __SILENTAUDIOCHUNK_H__

#include "iostream"
#include "SilentAudioChunk.h"

using namespace std;

SilentAudioChunk::SilentAudioChunk(uint32_t samplingRate, uint32_t duration) : AudioChunk(samplingRate, duration)
{
	cout << "Create a new SilentAudioChunk" << endl;
	this->samplesBuffer = new uint16_t[7];
	this->samplesBuffer[0] = 14;
}

SilentAudioChunk::~SilentAudioChunk()
{
//	cout << "Destroy SilentAudioChunk" << endl;

}

void SilentAudioChunk::genrate_samples()
{
	//Write the length of the Audio chunk in the fisrt byte
	this->samplesBuffer[0] = 0x14;

	//Write ehe ASCII character string "slnt" in the first 2 words = chunkid
	this->samplesBuffer[1] = 0x736c; //"sl"
	this->samplesBuffer[2] = 0x6e74; //"nt"

	this->samplesBuffer[3] = 0x0000;
	this->samplesBuffer[4] = 0x0004;
	this->samplesBuffer[5] = this->samplesNumber;
}
void SilentAudioChunk::printSilent()
{
	for (int i = 0; i < 7; i++){
		cout << std::hex <<this->samplesBuffer[i]<<" ";
	}
}

#endif // __SILENTAUDIOCHUNK_H__
