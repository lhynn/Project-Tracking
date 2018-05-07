#include "FimcConvert.h"

#include <stdio.h>
#include <string.h>

typedef struct myrgb32_st{
	char R;
	char G;
	char B;
	char M;
}P_MYRGB32;

MyFimcConvert::MyFimcConvert(){
	inbuf_vir = NULL;
}

MyFimcConvert::~MyFimcConvert(){
	Close();
}

int MyFimcConvert::open(){

	if(m_memtool.open() < 0){
		printf("faild to use sec-mem.\n");
		return -1;
	}
	m_addr.size = IWIDTH*IHEIGHT*6;

	if(m_memtool.aquire(&m_addr) < 0){
		printf("faild to aquire.\n");
		return -1;
	}

	inlen  = IWIDTH*IHEIGHT*4;
	outlen = IWIDTH*IHEIGHT*3/2;
	inbuf_phy = (char *) m_addr.phy_addr;
	inbuf_vir = (char *) m_addr.vir_addr;

	outbuf_phy = inbuf_phy + inlen;
	outbuf_vir = inbuf_vir + inlen;
	printf("-----m_addr.phy_addr:%d----m_addr.vir_addr:%p",m_addr.phy_addr,m_addr.vir_addr);

	if(!m_fimc.create(SecFimc::DEV_2,SecFimc::MODE_MULTI_BUF,1))
	{
		printf("faild to fimc.create.\n");
		return -1;	
	}

	unsigned int width 	= IWIDTH;
	unsigned int height 	= IHEIGHT;
	unsigned int cropWidth  = width,cropHeight = height;
	
	m_fimc.setSrcParams(width,height,0,0,&cropWidth,&cropHeight,HAL_PIXEL_FORMAT_RGBX_8888);//YCrCb_420_SP);

	char  *MyInbufPhy_1[IWIDTH*IHEIGHT];
	char  *MyInbufPhy_2[IWIDTH*IHEIGHT];
	char  *MyInbufPhy_3[IWIDTH*IHEIGHT];
	P_MYRGB32 * Tmp = (P_MYRGB32*)inbuf_phy;

	int i,j;
	for(j = 0;j < IHEIGHT;j++ ){
		for(i = 0;i < IWIDTH;i++){
			MyInbufPhy_1[j*IWIDTH + i] =&(Tmp->R);
			MyInbufPhy_2[j*IWIDTH + i] =&(Tmp->G);
			MyInbufPhy_3[j*IWIDTH + i] =&(Tmp->B); 
			Tmp++;
		}
	}
	m_fimc.setSrcAddr((unsigned int)*MyInbufPhy_1,(unsigned int)*MyInbufPhy_2,(unsigned int)*MyInbufPhy_3,0);
	m_fimc.setRotVal(0);
	m_fimc.setDstParams(width,height,0,0,&cropWidth,&cropHeight,HAL_PIXEL_FORMAT_CUSTOM_YCbCr_420_SP);//RGBA_8888);
	
	char  *MyOutBufPhy_1[IWIDTH*IHEIGHT/4];
	char  *MyOutBufPhy_2[IWIDTH*IHEIGHT/4];
	int n,m;
	
	for( n = 0;n < IHEIGHT/4;n++){
		for( m = 0;m < IWIDTH;){
			MyOutBufPhy_1[n*IHEIGHT/4 + m] = outbuf_phy + IWIDTH*IHEIGHT - 1 + m;
			m++;
			MyOutBufPhy_2[n*IHEIGHT/4 + m] = outbuf_phy + IWIDTH*IHEIGHT  - 1 + m;
			m++;
			}
	}
	m_fimc.setDstAddr((unsigned int)outbuf_phy,(unsigned int)*MyOutBufPhy_1,(unsigned int)*MyOutBufPhy_2,0);//,0,0,0);

	printf("set phy addr.\n");

	return 0;
}
void MyFimcConvert::Close(){
	if(inbuf_vir != NULL){
		inbuf_vir = NULL;
		m_memtool.release(&m_addr);
		m_memtool.close();
		m_fimc.destroy();
	}
}

int MyFimcConvert::Convert(){
	int ret;
	ret = m_fimc.draw(0,0);
	if(ret == false)
	{
		printf("m_fimc_draw faild.\n");
		return -1;
	}
	return 0;
}

