#ifndef __AUDIOCHUNK_H__
#define __AUDIOCHUNK_H__

#include <cstdint>
#include <cstring>

class AudioChunk{
protected :
		uint16_t * samplesBuffer;
		uint32_t samplingRate;
		uint32_t duration;
		uint32_t samplesNumber;
public :
		//Creates an empty AudioChunk
		AudioChunk(uint32_t samplingRate, uint32_t duration);
		//Creates an AudioChunk and Manually Fill the AudioChunk from a samplesBuffer
		AudioChunk(uint32_t samplingRate, uint32_t samplesNumber, uint16_t * samplesBuffer);
		//Destructor for freeing samplesBuffer
		~AudioChunk();
		//Getter for AudioChunk samplingRate
		uint32_t getSamplingRate();
		//Setter for AudioChunk samplingRate
		void setSamplingRate(uint32_t);
		//Getter for AudioChunk SamplesNumber
		uint32_t getSamplesNumber();
		//Setter for AudioChunk setSamplesNumber
		void setSamplesNumber();
		//Getter for AudioChunk duration
		uint32_t getDuration();
		//Setter for AudioChunk duration
		void setDuration(uint32_t);
		//Setter for AudioChunk duration
		void setSamplesNumber(uint32_t);
		//Truncate the AudioChunk samplesBuffer
		void TruncateBuffer(uint32_t startTime, uint32_t endTime);
		//Display the AudioChunk samplesBuffer
		void printBuffer();
};
#endif // __AUDIOCHUNK_H__
