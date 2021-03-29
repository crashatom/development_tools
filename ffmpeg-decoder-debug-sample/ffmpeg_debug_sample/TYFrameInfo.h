#ifndef __TYFrameInfo_H__
#define __TYFrameInfo_H__

#include <memory>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif

#include "TYPublic.h"
#include "TYAVDefines.h"

// 解码前的视频数据包
typedef struct tagTYVideoPacketInfo{
    tagTYVideoPacketInfo(){}
    
    tagTYVideoPacketInfo(int codecid, int nRequestId_, int nFrameRate_, int nIsIFrame_, int nWidth_, int nHeight_, unsigned long long lldCustomTimestamp_, unsigned int nRtpTimeStamp_, long long nDuration, long long nProgress, const unsigned char *pData, int nDataSize_){
        codec_id           = (TY_AV_CODEC_ID)codecid;
        nFrameRate         = nFrameRate_;
        nIsIFrame          = nIsIFrame_;
        nWidth             = nWidth_;
        nHeight            = nHeight_;
        nRtpTimeStamp      = nRtpTimeStamp_;
        nDataSize          = nDataSize_;
        nRequestId         = nRequestId_;
        sei                = NULL;      // init sei info
        uuid               = NULL;
        nSeiSize           = 0;
        nUuidSize          = 0;
        Progress.nDuration = nDuration;
        Progress.nProgress = nProgress;
        lldCustomTimestamp = lldCustomTimestamp_;
        
//        if (nDataSize_ > 0){
//            data.reset(new uint8_t[nDataSize_]);
//            if (data.get() != nullptr){
//                memcpy(data.get(), pData, nDataSize);
//            }
//        }
    }
    
    ~tagTYVideoPacketInfo(){
        if (sei){
            delete [] sei;
            sei = NULL;
        }
        nSeiSize = 0;
    }
    
    TY_AV_CODEC_ID codec_id;
    
    int nWidth;
    int nHeight;
    int nIsIFrame;
    int nFrameRate;
    int nRequestId;
    unsigned int nRtpTimeStamp;
    unsigned long long lldCustomTimestamp;
    
    uint8_t *data;
    unsigned int nDataSize;
    // sei info
    char *sei ;
    int nSeiSize ;
    char *uuid ;
    int nUuidSize ;
    
    struct{
        long long nProgress;
        long long nDuration;
    }Progress;
}TYVideoPacketInfo;

// 解码前的音频数据包
typedef struct tagTYAudioPacketInfo{
    tagTYAudioPacketInfo(int codecid, int nRequestId_, int nSampRate_, int nChannels_, int nBitWidth_, unsigned long long lldCustomTimeStamp_, unsigned int nRtpTimeStamp_, const unsigned char *pData, int nDataSize_){
        codec_id           = (TY_AV_CODEC_ID)codecid;
        nSampRate          = nSampRate_;
        nChannels          = nChannels_;
        nBitWidth          = nBitWidth_;
        nDataSize          = nDataSize_;
        nRequestId         = nRequestId_;
        nRtpTimeStamp      = nRtpTimeStamp_;
        lldCustomTimeStamp = lldCustomTimeStamp_;
        
        data.reset(new uint8_t[nDataSize_]);
        memcpy(data.get(), pData, nDataSize);
    }
    
    ~tagTYAudioPacketInfo(){
        
    }
    
    
    TY_AV_CODEC_ID codec_id;
    
    int nSampRate;
    int nChannels;
    int nBitWidth;
    unsigned long long lldCustomTimeStamp;
    unsigned int nRtpTimeStamp;
    
    std::shared_ptr<uint8_t> data;
    int nDataSize;
    int nRequestId;
}TYAudioPacketInfo;

// 解码后的音频数据帧
typedef struct tagTYAudioFrameInfo{
    tagTYAudioFrameInfo(){}
    tagTYAudioFrameInfo(int codecid, int nRequestId_, int nSampRate_, int nChannels_, int nBitWidth_, \
                        unsigned long long lldCustomTimeStamp_, unsigned int nRtpTimeStamp_, \
                        const unsigned char *pData, int nDataSize_){
        codec_id  = (TY_AV_CODEC_ID)codecid;
        nSampRate = nSampRate_;
        nChannels = nChannels_;
        nBitWidth = nBitWidth_;
        nDataSize = nDataSize_;
        nRtpTimeStamp = nRtpTimeStamp_;
        lldCustomTimeStamp = lldCustomTimeStamp_;
        nRequestId = nRequestId_;
        
        // 版本3.12.6 3.12.7 出现分配内存crash，未找到具体原因
        // 增加在线定位日志
        // 对音频包大于10k的情况做上报处理
        if (nDataSize_ < 10240){
            data.reset(new uint8_t[nDataSize_]);
            memcpy(data.get(), pData, nDataSize);
        }else{
            char szBuf[1024] = {0};
            sprintf(szBuf, "{\"memory_alloc_abnormal\":\"struct tagTYAudioFrameInfo\",\"exception_code\":\"new uint8_t[%d]\"}", nDataSize_);
            // TYLogManager::SendApmOnlineLog(TY_EVENT_IPC_CXX_SDK_EXTRA_DATA, szBuf);
        }
    }
    
    ~tagTYAudioFrameInfo(){
        
    }
    
    TY_AV_CODEC_ID codec_id;

    int nDataSize;
    int nSampRate;
    int nChannels;
    int nBitWidth;
    int nRequestId;
    unsigned int nRtpTimeStamp;
    unsigned long long lldCustomTimeStamp;
    
    std::shared_ptr<uint8_t> data;
}TYAudioFrameInfo;

// 解码后的视频数据帧
typedef struct tagTYVideoFrameInfo{
    struct{
        long long nProgress;
        long long nDuration;
    }Progress;
    
    tagTYVideoFrameInfo(){}
    tagTYVideoFrameInfo(int codecid, int nRequestId_, int nFrameRate_, int nIsIFrame_, int nWidth_, \
                        int nHeight_, unsigned long long lldCustomTimestamp_, unsigned int nRtpTimeStamp_, \
                        const unsigned char *pData, int nDataSize_){
        y                  = NULL;
        u                  = NULL;
        v                  = NULL;
        codec_id           = (TY_AV_CODEC_ID)codecid;
        nFrameRate         = nFrameRate_;
        nIsIFrame          = nIsIFrame_;
        nWidth             = nWidth_;
        nHeight            = nHeight_;
        nRtpTimeStamp      = nRtpTimeStamp_;
        nDataSize          = nDataSize_;
        nRequestId         = nRequestId_;
        lldCustomTimestamp = lldCustomTimestamp_;
        data = (unsigned char *)pData;
    }

    tagTYVideoFrameInfo(int codecid, int nRequestId_, int nFrameRate_, int nIsIFrame_, int nWidth_, \
                        int nHeight_, unsigned long long lldCustomTimestamp_, unsigned int nRtpTimeStamp_, \
                        const unsigned char *y_, const unsigned char *u_, const unsigned char *v_){
        codec_id           = (TY_AV_CODEC_ID)codecid;
        nFrameRate         = nFrameRate_;
        nIsIFrame          = nIsIFrame_;
        nWidth             = nWidth_;
        nHeight            = nHeight_;
        nRtpTimeStamp      = nRtpTimeStamp_;
        data               = NULL;
        nDataSize          = 0;
        nRequestId         = nRequestId_;
        lldCustomTimestamp = lldCustomTimestamp_;

        y.reset(new uint8_t[nWidth * nHeight]);
        memcpy(y.get(), y_, nWidth * nHeight);

        u.reset(new uint8_t[nWidth * nHeight / 4]);
        memcpy(u.get(), u_, nWidth * nHeight / 4);

        v.reset(new uint8_t[nWidth * nHeight / 4]);
        memcpy(v.get(), v_, nWidth * nHeight / 4);
    }
    
    ~tagTYVideoFrameInfo(){
#ifdef __APPLE__
        CFRelease(CFTypeRef(data));
#endif
    }
    
    void *data;
    
    int nWidth;
    int nHeight;
    int nIsIFrame;
    int nFrameRate;
    int nRequestId;
    TY_AV_CODEC_ID codec_id;
    unsigned int nDataSize;
    unsigned int nRtpTimeStamp;
    unsigned long long lldCustomTimestamp;

    std::shared_ptr<uint8_t> y;
    std::shared_ptr<uint8_t> u;
    std::shared_ptr<uint8_t> v;
}TYVideoFrameInfo;

typedef struct tagTYVideoFrameContainer{
    tagTYVideoFrameContainer(){
        videoFrameInfo.reset();
        videoPacketInfo.reset();
    }
    
    ~tagTYVideoFrameContainer(){
    }

    std::shared_ptr<TYVideoFrameInfo>  videoFrameInfo;
    std::shared_ptr<TYVideoPacketInfo> videoPacketInfo;
}TYVideoFrameContainer;

typedef struct tagTYAudioFrameContainer{
    tagTYAudioFrameContainer(){
        audioFrameInfo.reset();
        audioPacketInfo.reset();
    }
    ~tagTYAudioFrameContainer(){
    }
    
    std::shared_ptr<TYAudioFrameInfo>  audioFrameInfo;
    std::shared_ptr<TYAudioPacketInfo> audioPacketInfo;
}TYAudioFrameContainer;

#endif // __TYFrameInfo_H__
