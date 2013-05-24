#ifndef _SERAPHIM_YUV_BASE_H
#define _SERAPHIM_YUV_BASE_H
extern "C"{
#include<stdint.h>
};
namespace Seraphim{
	class YuvBase{
	private:
		int width;
		int height;
		int count;
		
	public:
		YuvBase(int _width,int _height,int _count):width(_width),height(_height),count(_count){};
		int getWidth(){return width;}
		int getHeight(){return height;}
		virtual long getSizePreFrame()=0;
		virtual uint8_t* getY()=0;
		virtual uint8_t* getU()=0;
		virtual uint8_t* getV()=0;
	};
};
#endif