#include "iostream"
#include "SineWaveAudioChunk.h"
#include "cmath"

using namespace std;

SineWaveAudioChunk::SineWaveAudioChunk(uint32_t samplingRate,
                                       uint32_t duration,
                                       uint32_t amplitude,
                                       uint32_t frequency,
                                       uint32_t phase):
AudioChunk(samplingRate,duration),
amplitude(amplitude),
frequency(frequency),
phase(phase)
{
	cout << "Create a new Sinwave Audio chunk, with amplitude = "
         << amplitude << ", frequency = " << frequency 
         << ", phase = " << phase << endl;
	this->samplesBuffer = new uint16_t[this->samplesNumber+1];
	this->samplesBuffer[0]=(2*this->samplesNumber)+1;
}

SineWaveAudioChunk::~SineWaveAudioChunk()
{
	// cout << "Destroy SineWaveAudioChunk" << endl;

}

uint32_t SineWaveAudioChunk::getamplitude()
{
	return(this->amplitude);
}

void SineWaveAudioChunk::setamplitude(uint32_t amplitude)
{
	this->amplitude = amplitude;
}

uint32_t SineWaveAudioChunk::getfrequency()
{
	return(this->frequency);
}

void SineWaveAudioChunk::setfrequency(uint32_t amplitude)
{
	this->frequency = frequency;
}

uint32_t SineWaveAudioChunk::getphase()
{
	return(this->amplitude);
}

void SineWaveAudioChunk::setphase(uint32_t)
{
	this->phase = phase;
}

void SineWaveAudioChunk::genrate_samples()
{
	double t = 0;
	this->samplesBuffer[0] = samplesNumber;
	for (int i = 1; i < samplesNumber; i++){
		this->samplesBuffer[i] = this->amplitude * sin(2 * M_PI * this->frequency * t + this->phase);
		t += 1/this->samplingRate;
	}
	cout << "generate sine wave !!" << endl;
}

