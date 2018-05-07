#ifndef __MYS3C_MEM_H__
#define __MYS3C_MEM_H__


#include <stdint.h>
#include <sys/ioctl.h>

#include "s3c_mem.h"

class Mys3cMem
{
	public:
		int m_fd;
		int open();
		void close();
		int aquire(s3c_mem_alloc *m);
		void release(s3c_mem_alloc *m);
};


#endif
