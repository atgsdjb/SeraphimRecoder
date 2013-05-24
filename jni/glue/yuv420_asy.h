#ifndef __SERAPHIM_YUV420_ASY_H
#define __SERAPHIM_YUV420_ASY_H
#include"yuv_base.h"
#include<queue>
#include<pthread.h>
namespace Seraphim{
	class Yuv420Asy :public YuvBase{
	private:
		std::queue<uint8_t*> buffer;
		long start_v;
		long start_u;
		uint8_t* pU;
		uint8_t* pV;
		//·½±ã²Ù×÷
		long sizePreY;
		long sizePreU;
		long sizePreV;
		//
		long postion;
		long sizePreFrame;
		unsigned long countFrame_r;
		unsigned long countFrame_w;
		unsigned long maxFrame;
		pthread_mutex_t mutex;
		pthread_mutex_t cond_mutex;
		pthread_cond_t  cond;

		void lock(){pthread_mutex_lock(&mutex);}
		void unlock(){pthread_mutex_unlock(&mutex);}
	public:
		Yuv420Asy(int _width,int _height,int _count);
		int getSize();
		long getSizePreFrame(){return sizePreFrame;};
		uint8_t* getY();
		uint8_t* getU();
		uint8_t* getV();
		virtual ~Yuv420Asy();
		int getCount(){return maxFrame;}
		bool isEmpty(){return buffer.empty();}
		bool notFull(){return maxFrame==-1?true:countFrame_w < maxFrame;};
		void addFrame(uint8_t* frame);
	};
};

#endif