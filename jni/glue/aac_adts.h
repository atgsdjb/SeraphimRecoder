extern"C"{
#include<stdint.h>
};
namespace Seraphim{
#define ADTS_HEADER_SIZE	7 
	/************************************************************************/
	/* 0: 96000 Hz
	1: 88200 Hz
	2: 64000 Hz
	3: 48000 Hz
	4: 44100 Hz
	5: 32000 Hz
	6: 24000 Hz
	7: 22050 Hz
	8: 16000 Hz
	9: 12000 Hz
	10: 11025 Hz
	11: 8000 Hz
	12: 7350 Hz                                                                     */
	/************************************************************************/
	enum AACSampleFrequencies{
		HZ96000=0,
		HZ88200,
		HZ64000,HZ48000,HZ44100,HZ32000,HZ24000,HZ22050,HZ16000,HZ12000,HZ11025,HZ8000,HZ7350
	};
	enum AACProfile{
		Main=0,LC
	};
class AdtsHelp{
private:
	uint8_t *adts;
	uint8_t t_byte;
	uint16_t indexBit;
	void put_bits(uint8_t countBit,uint32_t data);
	void flush_adts();
	AACProfile profile;
	AACSampleFrequencies sampleRate;
	
public:
	AdtsHelp(AACProfile _profile,AACSampleFrequencies _sampleRate):profile(_profile),sampleRate(_sampleRate),t_byte(0),indexBit(0){adts = new uint8_t[7];};
	int   adts_write_frame_header(uint8_t** dst ,uint8_t* sample ,size_t sizeSample);
};
};
