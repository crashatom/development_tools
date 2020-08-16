//
//  TYVideoCodecSelector.h
//  MediaCodec
//
//  Created by Jim on 19/7/15.
//  Copyright (c) 2019年 Tuya. All rights reserved.
//
//  Description: Android MediaCodec 解码器

#ifndef __TY_CODEC_SELECTOR_H__
#define __TY_CODEC_SELECTOR_H__

#include "TYFrameInfo.h"
#include "TYBaseVideoDecoder.h"

class TYVideoCodecSelector{
public:
    static TYBaseVideoDecoder *CreateDynamicVideoDecoder(std::shared_ptr<TYVideoPacketInfo> videoPacketInfo, void *userdata);
    static int DestroyDynamicVideoDecoder(TYBaseVideoDecoder **videoDecoder);
};

#endif  // __TY_CODEC_SELECTOR_H__
