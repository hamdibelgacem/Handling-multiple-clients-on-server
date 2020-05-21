#ifndef __AUDIOCHUNK_H__
#define __AUDIOCHUNK_H__

#include <cstdint>
#include <cstring>

class AudioChunk{
protected :
		uint32_t samplingRate;
		uint32_t duration;
		uint32_t samplesNumber;
		uint16_t * samplesBuffer;
public :
		//Creates an empty AudioChunk
		AudioChunk(double samplingRate, double duration);
		//Destructor for freeing samplesBuffer
		~AudioChunk();
		//Getter for AudioChunk samplingRate
		double getSamplingRate();
		//Setter for AudioChunk samplingRate
		void setSamplingRate(double);
		//Getter for AudioChunk SamplesNumber
		uint32_t getSamplesNumber();
		//Setter for AudioChunk setSamplesNumber
		void setSamplesNumber(uint32_t);
		//Getter for AudioChunk duration
		double getDuration();
		//Setter for AudioChunk duration
		void setDuration(double);
		//Getter for AudioChunk SamplesBuffer
		uint16_t * getSamplesBuffer();
		//Setter for AudioChunk SamplesBuffer
		void setSamplesBuffer(double samplesNumber, uint16_t * samplesBuffer);
		//Truncate the AudioChunk samplesBuffer
		void TruncateBuffer(double startTime, double endTime);
		//Display the AudioChunk samplesBuffer
		void printBuffer();
};
#endif // __AUDIOCHUNK_H__