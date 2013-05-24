extern"C"{
#include"pthread.h"
#include<stdint.h>
};
#include<deque>
using namespace std;
namespace Seraphim{
	class SyncBuffer{
	private:
		pthread_mutex_t mutex;
		pthread_cond_t  cond;
		pthread_mutex_t condMutex;
		deque<uint8_t*> d_buf;
		deque<int>		size_buf;
		bool endFlg;
	public:
		void write23(uint8_t* src,size_t size);
		int	 read(uint8_t** dst); 
		void writeBack(uint8_t* data,size_t size);
		void disable();
		SyncBuffer();
		~SyncBuffer();		
	};
};
