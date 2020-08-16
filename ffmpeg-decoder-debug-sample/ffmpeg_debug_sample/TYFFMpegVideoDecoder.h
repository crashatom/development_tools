//
//  FFMpegH264Decoder.h
//  H264Decode
//
//  Created by Jim on 19/5/30.
//  Copyright (c) 2019年 Tuya. All rights reserved.
//
//  Description: ffmpeg解码器

#ifndef __FFMPEG_H264_DECODER_H__
#define __FFMPEG_H264_DECODER_H__

#include <queue>
#include <pthread.h>

#include "TYVideoCodecSelector.h"

extern "C"{
    #include "opt.h"
    #include "avcodec.h"
    #include "channel_layout.h"
    #include "avformat.h"
    #include "common.h"
    #include "imgutils.h"
    #include "mathematics.h"
    #include "samplefmt.h"
}

class TYFFMpegVideoDecoder: public TYBaseVideoDecoder{
public:
    TYFFMpegVideoDecoder(const char *mime);
    ~TYFFMpegVideoDecoder();
public:
    int Init(const char *mime);
    int Uninit();
    static bool DecoderSupported(const char *mime);
    
    int DecodeNetStreamWithData(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata);

private:
    uint8_t              *y;
    uint8_t              *u;
    uint8_t              *v;

public:
    int                  m_nYUVLength;
    bool                 m_bInit;
    int                  m_nWidth;
    int                  m_nHeight;
    uint8_t              *m_pYUVData;
    AVCodec              *m_pCodec;
    AVFrame              *m_pDecodedFrame;
    uint8_t              *m_pDecodedBuffer;
    AVCodecContext       *m_pCodecContext;
    AVFormatContext      *m_pFormatContext;
    AVCodecParserContext *m_pParser;
    pthread_mutex_t      m_codecMutex;          // 编码器初始化，反初始化及解码时应该上的锁(否则解码时context可能已被uninit)
    
    char                 m_szMime[64];
};

#endif  // __FFMPEG_H264_DECODER_H__
