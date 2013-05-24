#include"aac_adts.h"
#include<stdlib.h>
#include<stdio.h>
extern"C"{
#include"us_log.h"
}
using namespace std;
namespace Seraphim{

void AdtsHelp::put_bits(uint8_t countBit,uint32_t data){
	int i ;
	for(i = countBit -1;i>=0;--i){
		int t_i = indexBit/8;
		adts[t_i] = adts[t_i]<<1;
		uint32_t l_t_t = (data>>i) &0x01;
		adts[t_i] |= (data>>i) & 0x01; 
		uint8_t t_t = adts[t_i];
		td_printf("--indexBit=%d---countBit=%d,data=%d---------------------\n",indexBit,countBit,t_t);
		indexBit++;
	}
}

/**
**/
void AdtsHelp::flush_adts(){

}


/************************************************************************/
/*                                                                      */
/************************************************************************/
int  AdtsHelp::adts_write_frame_header(uint8_t** dst ,uint8_t* sample ,size_t sizeSample)
{
	uint8_t pb;
	//PutBitContext pb;

	//init_put_bits(&pb, buf, ADTS_HEADER_SIZE);

	/* adts_fixed_header */
//	countBit=0;
	indexBit = 0;
	memset(adts,0,ADTS_HEADER_SIZE);
	put_bits(12, 0xfff);   	/* syncword */
	put_bits(1, 0);        	/* ID */
	put_bits(2, 0);        	/* layer */
	put_bits(1, 1);        	/* protection_absent */
	put_bits(2,profile ); 	/* profile_objecttype */
	put_bits(4, sampleRate);
	put_bits(1, 0);        /* private_bit */
	put_bits(3, 1); 		/* channel_configuration */
	put_bits(1, 0);        /* original_copy */
	put_bits(1, 0);        /* home */

	/* adts_variable_header */
	put_bits(1, 0);        /* copyright_identification_bit */
	put_bits(1, 0);        /* copyright_identification_start */
	put_bits(13, ADTS_HEADER_SIZE + sizeSample /*+ pce_size */); /* aac_frame_length */
	put_bits(11, 0x7ff);   /* adts_buffer_fullness */
	put_bits(2, 0);        /* number_of_raw_data_blocks_in_frame */
	uint8_t* l_dst = new uint8_t[ADTS_HEADER_SIZE+sizeSample];
	memcpy((void*)l_dst,(void*)adts,ADTS_HEADER_SIZE);
	memcpy((void*)(l_dst+ADTS_HEADER_SIZE),(void*)sample,sizeSample);
	*dst = l_dst;
	return sizeSample+ADTS_HEADER_SIZE;
}
};
