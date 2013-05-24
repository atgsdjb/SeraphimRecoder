#include"SBitReader.h"
#include"s_config.h"
namespace Seraphim{
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	inline static int getByteNumOfBitNum(int bitNum){
		int len = -1;
		if(bitNum<=8){
			len = 1;
		}else if(bitNum <=16){
			len = 2;
		}else if(bitNum <=32){
			len =4;
		}else if(bitNum <=64){
			len = 8;
		}
		return len;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	inline static uint8_t getHClearMask(uint8_t data,uint8_t num){
		uint8_t i = 1;
		uint8_t result = 0x00;
		for(i;i <= num;i++){
			result >>=1;
			result |= 0x80;
		}
		return (~result) & data;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	inline static uint8_t getLClearMask(uint8_t data ,uint8_t num){
		uint8_t i = 1;
		uint8_t result = 0x00;
		for(i;i <= num;i++){
			result <<=1;
			result |= 0x01;
		}
		return (~result) & data;
	}


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	inline static uint8_t getHSetMask(uint8_t data,uint8_t num){
		uint8_t i = 1;
		uint8_t result = 0x00;
		for(i;i <= num;i++){
			result >>=1;
			result |= 0x80;
		}
		return result;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	inline static uint8_t getLSetMask(uint8_t data,uint8_t num){
		uint8_t i = 1;
		uint8_t result = 0x00;
		for(i;i <= num;i++){
			result <<=1;
			result |= 0x01;
		}
		return result;
	}
	inline static void verify(uint8_t* d){
		uint8_t l_d = *d %8+1;
		*d =l_d;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int SBitReader::read(uint8_t* dst,int bitNum){
		int byteIndex=0;
		while(bitNum>=8){
			dst[byteIndex] = buf[positionByte] <<(8-positionBit);
			positionByte++;
			if(8>positionBit)   //may not be used
				dst[byteIndex] |= buf[positionByte] >> positionBit;
			byteIndex++;
			bitNum -=8;
		}
		if(bitNum>0){
			if(positionBit >= bitNum){
				size_t  l_pBy= positionByte;
				dst[byteIndex] = getHClearMask(buf[positionByte],8-positionBit) >>(positionBit-bitNum);
				positionBit -= bitNum;
				
			}else{
				dst[byteIndex] = getHClearMask(buf[positionByte++],8-positionBit) <<(bitNum - positionBit);
				dst[byteIndex] |= buf[positionByte] >>8+positionBit-bitNum;// 8-(bitNum-positionBit)
				positionBit = 8+positionBit - bitNum;

			}
		}


		return (len-positionByte-1)+positionBit;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int SBitReader::readByte(uint8_t* dst,uint8_t numBit){
		read(dst,numBit);
		return (len-positionByte)*8-positionBit;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int SBitReader::readShort(uint16_t* dst,uint8_t numBit,bool isBigEnd){
		uint8_t t_b[2]={0,0};
		uint16_t t_d = 0x0000;
		int i =0;
		read(t_b,numBit);
		if(isBigEnd){
			while(true){
				t_d |= t_b[i++];
				if(i==sizeof(int)){
					break;
				}
				t_d <<=8;
			}
		}else{

		}
		return (len-positionByte)*8-positionBit;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int SBitReader::readInt(uint32_t* dst,uint8_t numBit,bool isBigEnd/* =true */){
		uint8_t t_b[4]={0,0,0,0};
		uint32_t t_d = 0x00000000;
		int i =0;
		read(t_b,numBit);
		if(isBigEnd){
			while(true){
				t_d |= t_b[i++];
				if(i==sizeof(int)){
					break;
				}
				t_d <<=8;
			}
		}else{

		}
		return (len-positionByte)*8-positionBit;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	int SBitReader::readLong(uint64_t* dst,uint8_t numBit,bool isBigEnd){
		return (len-positionByte)*8-positionBit;
	}
}