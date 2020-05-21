#include "iostream"
#include "AudioChunk.h"

using namespace std;


AudioChunk::AudioChunk(double samplingRate, double duration) : 
samplingRate(samplingRate),
duration(duration),
samplesNumber(samplingRate * duration),
samplesBuffer(NULL)
{
	cout << "Create a new Empty Audio chunk, with sampling rate = "
         << samplingRate << "and duration = " << duration << endl;
}

AudioChunk::~AudioChunk()
{
	if (this->samplesBuffer){
		delete[] this->samplesBuffer;
		this->samplesBuffer=NULL;
	}
}

double AudioChunk::getSamplingRate()
{
	return (this->samplingRate);
}

void AudioChunk::setSamplingRate(double samplingRate)
{
	this->samplingRate = samplingRate;
}

double AudioChunk::getDuration()
{
	return (this->duration);
}

void AudioChunk::setDuration(double duration)
{
	this->duration = duration;
	this->samplesNumber = this->samplingRate*this->duration;
}

uint32_t AudioChunk::getSamplesNumber()
{
	return(this->samplesNumber);
}

void AudioChunk::setSamplesNumber(uint32_t samplesNumber)
{
	this->samplesNumber = samplesNumber;
	this->duration = this->samplesNumber/this->samplingRate;
}

uint16_t * AudioChunk::getSamplesBuffer()
{
	return(this->samplesBuffer);
}

void AudioChunk::setSamplesBuffer(double samplesNumber, uint16_t * samplesBuffer)
{
	this->samplesBuffer = new uint16_t[this->samplesNumber+1];
	this->samplesBuffer[0]=(2*this->samplesNumber)+1;
	memcpy(this->samplesBuffer+1, samplesBuffer, samplesNumber);
}

void AudioChunk::TruncateBuffer(double startTime, double endTime)
{
	double t = 0;
	unsigned int i = 1, j = 0;
	while (i < this->samplesNumber && t <endTime) {
		if (t >= startTime) {
			j++;
			this->samplesBuffer[j]=this->samplesBuffer[i];
		}
		i ++;
		t += 1/this->samplingRate;
	}
	this->samplesBuffer[0]=j;
}

void AudioChunk::printBuffer()
{
	for (unsigned int i = 0; i < this->samplesNumber; i++){
		cout << "0x" << hex << this->samplesBuffer[i];
	}
}
