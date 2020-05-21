#ifndef __SILENTAUDIOCHUNK_H__
#define __SILENTAUDIOCHUNK_H__

#include "iostream"
#include "SilentAudioChunk.h"

using namespace std;

SilentAudioChunk::SilentAudioChunk(double samplingRate, double duration):
AudioChunk(samplingRate, duration)
{
	cout << "Create a new SilentAudioChunk" << endl;
	this->samplesBuffer = new uint16_t[7];
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
	cout << "before " << hex << this->samplesNumber << endl;
	uint32_t x = (this->samplesNumber & 0xFFFF0000) >> 16 |  (this->samplesNumber & 0x0000FFFF) << 16;
	cout << "after " << hex << x << endl;
	memcpy(samplesBuffer+5, &x, 4);
}

#endif // __SILENTAUDIOCHUNK_H__
