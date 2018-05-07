#include "mfcmanager.h"
#include <stdlib.h>
#include <stdio.h>

static PT_MyMfcOpr g_tMyMfcOprHead = NULL;

void RegisterMfcDevice(PT_MyMfcOpr pt_yMfcOpr){
	 PT_MyMfcOpr  ptTmp;
    if(!g_tMyMfcOprHead){
        g_tMyMfcOprHead = pt_yMfcOpr;
        pt_yMfcOpr->Next = NULL;
    }
    else{
        ptTmp = g_tMyMfcOprHead;
        while(ptTmp->Next){
            ptTmp = ptTmp->Next;
        }
        ptTmp->Next = pt_yMfcOpr;
        pt_yMfcOpr->Next = NULL;
    }
}


SSBSIP_MFC_ERROR_CODE InitMfcDevice(int w, int h, int qb){
	SSBSIP_MFC_ERROR_CODE ret;
	PT_MyMfcOpr ptTmp = g_tMyMfcOprHead;

	while(ptTmp)
	{
		ret =  ptTmp->initMfc(w,h,qb);
		ptTmp = ptTmp->Next;
	}
	return ret;
}
int GetDateHeader(unsigned char** p){
	int ret;
	PT_MyMfcOpr ptTmp = g_tMyMfcOprHead;

    while(ptTmp)
    {
        ret =  ptTmp->getHeader(p);
		ptTmp = ptTmp->Next;
    }
    return ret;

}
void GetInputBuf(void **Y,void **UV){

	PT_MyMfcOpr ptTmp = g_tMyMfcOprHead;

    while(ptTmp)
    {
        ptTmp->getInputBuf(Y,UV);
        ptTmp = ptTmp->Next;
    }
}
int Encode(void *input_buf,void **output_buf){
	
	int ret;
	PT_MyMfcOpr ptTmp = g_tMyMfcOprHead;

    while(ptTmp)
    {
        ret =  ptTmp->encode(input_buf,output_buf);
        ptTmp = ptTmp->Next;
    }
	return ret;
}
void ExitMfc(void){

    PT_MyMfcOpr ptTmp = g_tMyMfcOprHead;

    while(ptTmp)
    {
        ptTmp->exitMfc();
        ptTmp = ptTmp->Next;
    }
}
