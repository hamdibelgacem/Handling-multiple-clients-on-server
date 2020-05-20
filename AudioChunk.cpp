#include "iostream"
#include "AudioChunk.h"

using namespace std;


AudioChunk::AudioChunk(uint32_t samplingRate, uint32_t duration) : 
samplingRate(samplingRate),
duration(duration),
samplesNumber(samplingRate * duration),
samplesBuffer(NULL)
{
	cout << "Create a new Empty Audio chunk, with sampling rate = "
         << samplingRate << "and duration = " << duration << endl;
}

AudioChunk::AudioChunk(uint32_t samplingRate, uint32_t samplesNumber, 
		               uint16_t * samplesBuffer) : 
samplingRate(samplingRate),
samplesNumber(samplesNumber),
duration(samplesNumber/samplingRate)

{
	cout << "Create a new Empty Audio chunk, with sampling rate = "
         << samplingRate << "and duration = " << duration << endl;
    //first word will store the audio chunk length
	this->samplesBuffer = new uint16_t[this->samplesNumber+1];
	this->samplesBuffer[0]=(2*this->samplesNumber)+1;
	memcpy(this->samplesBuffer+1, samplesBuffer, samplesNumber);
}

AudioChunk::~AudioChunk()
{
	if (this->samplesBuffer){
		delete[] this->samplesBuffer;
		this->samplesBuffer=NULL;
	}
}

uint32_t AudioChunk::getSamplingRate()
{
	return (this->samplingRate);
}

void AudioChunk::setSamplingRate(uint32_t samplingRate)
{
	this->samplingRate = samplingRate;
}

uint32_t AudioChunk::getDuration()
{
	return (this->duration);
}

void AudioChunk::setDuration(uint32_t duration)
{
	this->duration = duration;
	this->samplesNumber = this->samplingRate*this->duration;
}

uint32_t AudioChunk::getSamplesNumber()
{
	return(this->samplesNumber);
}

void AudioChunk::setSamplesNumber()
{
	this->samplesNumber = samplesNumber;
	this->duration = this->samplesNumber/this->samplingRate;
}

void AudioChunk::TruncateBuffer(uint32_t startTime, uint32_t endTime)
{
	double t = 0;
	int i = 1, j = 0;
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
	for (int i = 0; i < this->samplesNumber; i++){
		cout << "0x" << hex << this->samplesBuffer[i];
	}
}
