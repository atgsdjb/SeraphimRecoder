#include"sync_buf.h"
namespace Seraphim{
	SyncBuffer::SyncBuffer():endFlg(false){
		mutex = PTHREAD_MUTEX_INITIALIZER;
		condMutex = PTHREAD_MUTEX_INITIALIZER;
		cond = PTHREAD_COND_INITIALIZER;

	}
	void SyncBuffer::write23(uint8_t* src,size_t size){
		pthread_mutex_lock(&mutex);
		d_buf.push_back(src);
		size_buf.push_back(size);
		pthread_mutex_lock(&condMutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&condMutex);
		pthread_mutex_unlock(&mutex);
	}
	int SyncBuffer::read(uint8_t** dis){
		pthread_mutex_lock(&condMutex);
		while(d_buf.empty()){
			if(endFlg){
				pthread_mutex_unlock(&condMutex);
				return -1;
			}
			pthread_cond_wait(&cond,&condMutex);
			if(endFlg){
				pthread_mutex_unlock(&condMutex);
				return -1;
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
	void SyncBuffer::writeBack(uint8_t *data,size_t size){
		pthread_mutex_lock(&mutex);
		d_buf.push_front(data);
		size_buf.push_front(size);
		pthread_mutex_unlock(&mutex);
	}
	void SyncBuffer::disable(){
		pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&condMutex);
		endFlg =true;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&condMutex);
	}
	SyncBuffer::~SyncBuffer(){
		pthread_mutex_destroy(&mutex);
		pthread_mutex_destroy(&condMutex);
		pthread_cond_destroy(&cond);
	}
};
