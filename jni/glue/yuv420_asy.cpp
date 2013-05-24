#include"yuv420_asy.h"
#include<iostream>
extern"C"{
#include"us_log.h"
};
using namespace  std;
namespace Seraphim{
	
	Yuv420Asy::Yuv420Asy(int _width,int _height,int _count):YuvBase(_width,_height,0),
		maxFrame(_count),countFrame_r(0),countFrame_w(0){
		mutex = PTHREAD_MUTEX_INITIALIZER;
		cond_mutex =PTHREAD_MUTEX_INITIALIZER;
		cond = PTHREAD_COND_INITIALIZER;
		long l_size = _width*_height;
		start_u = l_size;
		start_v = l_size*5/4;
		sizePreY = l_size;
		sizePreU= l_size/4;
		sizePreV = sizePreU;
		sizePreFrame = sizePreY+sizePreU+sizePreV;

	}
	uint8_t* Yuv420Asy::getY(){
		if(maxFrame!=-1 && countFrame_r==maxFrame){
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		while(buffer.empty()){
			//pthread_mutex_unlock(&mutex);
			pthread_mutex_lock(&cond_mutex);
			pthread_cond_wait(&cond,&cond_mutex);
			pthread_mutex_unlock(&cond_mutex);
		}
		pthread_mutex_lock(&mutex);
		uint8_t *pY = buffer.front();
		buffer.pop();
		pU = pY+start_u;
		pV = pY+start_v;
		countFrame_r++;
		pthread_mutex_unlock(&mutex);
		return pY;
	}
	uint8_t* Yuv420Asy::getU(){
		pthread_mutex_lock(&mutex);
		uint8_t* result = pU;
		pthread_mutex_unlock(&mutex);
		return result;
	}
	uint8_t* Yuv420Asy::getV(){
		pthread_mutex_lock(&mutex);
		uint8_t* result = pU;
		pthread_mutex_unlock(&mutex);
		return result;

	}
	void Yuv420Asy::addFrame(uint8_t* _frame){
		pthread_mutex_lock(&mutex);
		buffer.push(_frame);
		countFrame_w++;
		pthread_mutex_lock(&cond_mutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&cond_mutex);
		pthread_mutex_unlock(&mutex);

	}
	int Yuv420Asy::getSize(){
		return buffer.size();
	}
	Yuv420Asy::~Yuv420Asy(){
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&cond_mutex);
		pthread_cond_destroy(&cond);
	}
};
