#ifndef _SERAPHIM_SYNC_BUFFER_H
#define _SERAPHIM_SYNC_BUFFER_H

#define MAX_WAIT_COUNT  10

extern"C"{
#include"pthread.h""
#include<stdint.h>
};
#include<deque>
using namespace std;

namespace Seraphim{
	class SSyncBuffer{
	private:
		pthread_mutex_t mutex;
		pthread_cond_t  cond;
		pthread_mutex_t condMutex;
		timespec* waitTime;
		deque<uint8_t*> d_buf;
		deque<int>		size_buf;
		bool endFlg;
	public:
		void write23(uint8_t* src,size_t size);
		int	 read(uint8_t** dst); 
		void disable();
		void writeBack(uint8_t* data,size_t size);
		SSyncBuffer();
		~SSyncBuffer();		
	};
};
#endif