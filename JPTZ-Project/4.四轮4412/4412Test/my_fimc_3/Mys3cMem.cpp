#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "Mys3cMem.h"

int Mys3cMem::open()
{
	m_fd = ::open("/dev/s3c-mem",O_RDWR);
	if(m_fd < 0)
	{
		printf("open s3c-mem error.\n");
		return -1;
	}
	return 0;
}

void Mys3cMem::close(){
	::close(m_fd);
}

int Mys3cMem::aquire(s3c_mem_alloc *m){
	m->phy_addr = m->vir_addr = 0;
//	int ret = ioctl(m_fd,S3C_MEM_CACHEABLE_ALLOC,m);//S3C_MEM_SHARE_ALLOC,m);
//	int ret = ioctl(m_fd,S3C_MEM_ALLOC,m);//S3C_MEM_SHARE_ALLOC,m);
	int ret = ioctl(m_fd,S3C_MEM_ALLOC,m);//S3C_MEM_SHARE_ALLOC,m);
	if(ret < 0)
	{
		printf("ctler s3c_mem faild.\n");
		return -1;
	}
	return 0;
}

void Mys3cMem::release(s3c_mem_alloc *m){
	ioctl(m_fd,S3C_MEM_FREE,m);
	m->phy_addr = m->vir_addr = 0;
}






