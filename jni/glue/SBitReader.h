#ifndef __SERAPHIM_BIT_READER_H
#define  __SERAPHIM_BIT_READER_H
#include<stdint.h>
namespace Seraphim{
	class SBitReader{
	private:
		uint8_t* buf;
		size_t   len;
		size_t   positionByte;
		uint8_t  positionBit;
	public:
		SBitReader(uint8_t* _buf,size_t _len):buf(_buf),len(_len),positionByte(0),positionBit(8){};
		int read(uint8_t* dst,int bitNum);
		int readByte(uint8_t* dst,uint8_t numBit);
		int readInt(uint32_t* dst,uint8_t numBit,bool isBitEnd=true);
		int readShort(uint16_t* dst,uint8_t numBit,bool isBitEnd=true);
		int readLong(uint64_t* dst,uint8_t numBit,bool isBitEnd=true);
		size_t getRemainSize(){return  (len-positionByte-1)+positionBit;}   
		~SBitReader(){delete[] buf;};
	};
};



#endif