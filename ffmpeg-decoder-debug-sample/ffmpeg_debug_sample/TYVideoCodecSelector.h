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
