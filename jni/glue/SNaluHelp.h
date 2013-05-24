#ifndef _SERAPHIM_NALU_HELP
#define _SERAPHIM_NALU_HELP
#include<cstdio>
#include<cstdint>
#include<vector>

using namespace std;
namespace Seraphim{
	/************************************************************************/
	/*
		result  0 ok,-1 start'bit error, - 2 end'bit error  
	*/
	/************************************************************************/
	enum nalu_head{
		NALU4H=4,NALU5H
	};
	int getNALUFormStream(uint8_t* stm,int lenStm,uint8_t** dist,nalu_head head=NALU4H,bool isCopy=true);
	bool isPPS(uint8_t* nalu,nalu_head head);
	bool isSPS(uint8_t* nalu,nalu_head head);
	int getNaluS(uint8_t* stm,int len,nalu_head head,vector<uint8_t*>& naluS,vector<int>& naluC);
	bool isIFrame(uint8_t *frame);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	class SNaluHelp{
	private :
		FILE* file;
		uint8_t* buf;
		uint8_t* postion;
		uint8_t* end_buf;
		size_t maxSize;
		bool fullBuffer();
		bool checkEmpty();
	public :
		SNaluHelp(char* fileName,size_t maxSize=1024*512);
		uint8_t* nextNALU();
		~SNaluHelp(){delete[] buf;fclose(file);}
	};

};
#endif