#include"SSyncBuf.h"
namespace Seraphim{
	SSyncBuffer::SSyncBuffer():endFlg(false){
		mutex = PTHREAD_MUTEX_INITIALIZER;
		condMutex = PTHREAD_MUTEX_INITIALIZER;
		cond = PTHREAD_COND_INITIALIZER;
		waitTime = new timespec;
		waitTime->tv_sec=0;
		waitTime->tv_nsec=10000;
	}
	void SSyncBuffer::write23(uint8_t* src,size_t size){
		pthread_mutex_lock(&mutex);
		d_buf.push_back(src);
		size_buf.push_back(size);
		pthread_mutex_lock(&condMutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&condMutex);
		pthread_mutex_unlock(&mutex);
	}
	int SSyncBuffer::read(uint8_t** dis){
		pthread_mutex_lock(&condMutex);
		int  waitCount = 0;
		while(d_buf.empty()){
			pthread_cond_timedwait(&cond,&condMutex,waitTime);
			waitCount++;
			if(endFlg){
				pthread_mutex_unlock(&condMutex);
				return -1;
			}
			if(waitCount>MAX_WAIT_COUNT){
				pthread_mutex_unlock(&condMutex);
				return 0;
			}
		}
		
		pthread_mutex_unlock(&condMutex);
		pthread_mutex_lock(&mutex);
		*dis = d_buf.front();
		int result = size_buf.front();
		d_buf.pop_front();
		size_buf.pop_front();
		pthread_mutex_unlock(&mutex);
		return result;
	}
	void SSyncBuffer::disable(){
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&condMutex);
		endFlg =true;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&condMutex);
	}
	SSyncBuffer::~SSyncBuffer(){
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&condMutex);
		pthread_cond_destroy(&cond);
	}
	void SSyncBuffer::writeBack(uint8_t *data,size_t size){
		pthread_mutex_lock(&mutex);
		d_buf.push_front(data);
		size_buf.push_front(size);
		pthread_mutex_unlock(&mutex);
	}


};