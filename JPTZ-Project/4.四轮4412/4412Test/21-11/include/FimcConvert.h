#ifndef __FIMCCONVERT_H_
#define __FIMCCONVERT_H_

#include "Mys3cMem.h"
#include "SecFimc.h"
#include "../videomanager.h"

class MyFimcConvert{
	
public:
	MyFimcConvert();
	~MyFimcConvert();

	int open();
	void Close();
	
	int Convert();
public:
	char *inbuf_vir,*outbuf_vir;
	char *inbuf_phy,*outbuf_phy;
	int inlen,outlen;
private:
	s3c_mem_alloc m_addr;
	SecFimc m_fimc;
	Mys3cMem m_memtool;
};



#endif
