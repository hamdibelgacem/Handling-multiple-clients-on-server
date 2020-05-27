#include "AudioChunk.h"

using namespace std;


AudioChunk::AudioChunk(double samplingRate, double duration) : 
samplingRate(samplingRate),
duration(duration),
samplesNumber(samplingRate * duration),
samplesBuffer(samplesNumber + 1)
{
	cout << "Create a new Empty Audio chunk, with sampling rate = "
         << samplingRate << "Hz and duration = " << duration << "s" << endl;
}

AudioChunk::~AudioChunk()
{
	this->samplesBuffer.clear();
	this->samplesNumber = 0;
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

std::vector<uint16_t> AudioChunk::getSamplesBuffer()
{
	return(this->samplesBuffer);
}

void AudioChunk::setSamplesBuffer(double samplesNumber, std::vector<uint16_t> samplesBuffer)
{	
	this->samplesBuffer[0] = (2*this->samplesNumber) + 1;	
	std::copy(samplesBuffer.begin() + 1, samplesBuffer.end(), (this->samplesBuffer).begin());
	
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
	for (unsigned int i = 0; i <= this->samplesNumber; i++){
		cout << std::hex << setw(4) << setfill('0') << this->samplesBuffer[i];
	}
	cout << endl;
}
