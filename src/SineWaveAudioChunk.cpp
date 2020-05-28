#include "SineWaveAudioChunk.h"

using namespace std;

SineWaveAudioChunk::SineWaveAudioChunk(double samplingRate,
                                       double duration,
                                       double amplitude,
                                       double frequency,
                                       double phase):
AudioChunk(samplingRate,duration),
amplitude(amplitude),
frequency(frequency),
phase(phase)
{
	cout << "Create a new Sinwave Audio chunk, with amplitude = "
         << amplitude << ", frequency = " << frequency 
         << ", phase = " << phase << endl;
	this->samplesBuffer.resize(this->samplesNumber + 1);
	this->samplesBuffer[0]=(2*this->samplesNumber)+1;
}

SineWaveAudioChunk::~SineWaveAudioChunk()
{
	// cout << "Destroy SineWaveAudioChunk" << endl;

}

double SineWaveAudioChunk::getamplitude()
{
	return(this->amplitude);
}

void SineWaveAudioChunk::setamplitude(double amplitude)
{
	this->amplitude = amplitude;
}

double SineWaveAudioChunk::getfrequency()
{
	return(this->frequency);
}

void SineWaveAudioChunk::setfrequency(double amplitude)
{
	this->frequency = frequency;
}

double SineWaveAudioChunk::getphase()
{
	return(this->amplitude);
}

void SineWaveAudioChunk::setphase(double phase)
{
	this->phase = phase;
}

void SineWaveAudioChunk::genrate_samples()
{
	double t = 0;
	this->samplesBuffer[0] = samplesNumber;
	for (unsigned int i = 1; i <= samplesNumber; i++){
		this->samplesBuffer[i] = this->amplitude * sin(2 * M_PI * this->frequency * t + this->phase);
		t = t+  ((double) 1/this->samplingRate);
	}
}

