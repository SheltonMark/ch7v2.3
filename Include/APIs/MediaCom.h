#ifndef __MEDIACOM_H__
#define __MEDIACOM_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct MediaCaps_s
{
	int spotcap;// spot输出
	int zerochncap;//零通道编码的能力
	int val[16];//预留
}MediaCaps_t,*MediaCaps_p;

int MediaGet960HCaps();
int MediaDeleteMmz();
int MediaPrepareFreeMem();
void* MediaGetMmzMem(unsigned int size);

int MediaGetConfigCaps(MediaCaps_t *caps);

#ifdef __cplusplus
}
#endif

#endif
