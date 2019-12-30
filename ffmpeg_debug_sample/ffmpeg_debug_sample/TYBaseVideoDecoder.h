//
//  TYBaseVideoDecoder.h
//  TYBaseVideoDecoder
//
//  Created by Jim on 19/7/15.
//  Copyright (c) 2019年 Tuya. All rights reserved.
//
//  Description: 解码器基类

#ifndef __TY_BASE_VIDEO_DECODER_H__
#define __TY_BASE_VIDEO_DECODER_H__

#include "TYUtils.h"
#include "TYFrameInfo.h"


#ifdef __APPLE__
#include <CoreVideo/CoreVideo.h>
#endif

class TYBaseVideoDecoderInterface{
public:
    virtual void onVideoSpecificDecodedDataCompleted(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *decodedData, void *userdata) {};
    virtual void onVideoYUVDataCompleted(void *contextRef, uint8_t *pEncodedData, int nEncodedDataSize, uint8_t *pY, uint8_t *pU, uint8_t *pV, bool bIsIFrame, int nFrameRate, int nWidth, int nHeight, unsigned long long timestamp, long long nTimeStamp, long long nDuration, long long nProgress, int nRequestId,char *sei,int seilen, void *userdata) = 0;
};


class TYBaseVideoDecoder{
public:
    TYBaseVideoDecoder();
    virtual ~TYBaseVideoDecoder();
    void GetSeiInfo(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,uint8_t *stream,int size);
    void draw_frames_for_image(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,uint8_t *y,uint8_t *u,uint8_t *v,int w,int h);
#ifdef __APPLE__
    void draw_one_frame_ios(CVImageBufferRef imageBuffer, objectRect objs);
    void draw_frames_for_image(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,CVImageBufferRef imageBuffer) ;
#endif
public:
    virtual int Init() { return 0; };
    virtual int Uninit() { return 0; };
    virtual int DecodeNetStreamWithData(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata) = 0;
    void EnableIVA(bool enable);
    void onLastVideoFrameArrived() ;
public:
    TYBaseVideoDecoderInterface *delegate;
    objectRect *activeObjs ;
    int nObjsNumbers ;
    bool m_bIVAEnable ;
    bool m_bLastFrameArrive ;
};

#endif  // __TY_BASE_VIDEO_DECODER_H__
