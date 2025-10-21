#ifndef __VIDEOPRIV_H__
#define __VIDEOPRIV_H__

#define MAX_VIDEO_IN_CHANNEL        32+2 //16


typedef struct AdecDevice_s
{
    
}AdecDevice_t, *AdecDevice_p;

typedef struct VideoInDevice_s
{
	int videoInChannel;
    
}VideoInDevice_t, *VideoInDevice_p;
#endif
