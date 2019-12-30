#include "TYPublic.h"
#include "TYiOSVideoHWDecoder.h"
#include "TYFFMpegVideoDecoder.h"
#include "TYVideoCodecSelector.h"

TYBaseVideoDecoder *TYVideoCodecSelector::CreateDynamicVideoDecoder(std::shared_ptr<TYVideoPacketInfo> videoPacketInfo, void *userdata){
    char szJson[640] = {0};
    TYBaseVideoDecoder *videoDecoder = NULL;

    const char *mime = NULL;
    switch(videoPacketInfo->codec_id){
        case TY_AV_CODEC_VIDEO_H264:
            mime = "video/avc";
            break;
        case TY_AV_CODEC_VIDEO_H265:
            mime = "video/hevc";
            break;
        default:
            LOGI("TYCameraSDK [ERROR]: codec not support: codec_id:%d\n", videoPacketInfo->codec_id);
    }

    // Select mediacodec/videotoolbox firstly, if not support, select ffmpeg secondly.
    // If ffmpeg is forbidden, select openH264 optionally.
#if defined(__ANDROID__) && !defined(__NO_MEDIACODEC__)
    char szCodecName[256] = {0};

    if (videoDecoder == NULL && TYMediaCodecVideoDecoder::HWDecodeSupported(mime, szCodecName, sizeof(szCodecName))){
        videoDecoder = new TYMediaCodecVideoDecoder(videoPacketInfo->codec_id);

        snprintf(szJson, sizeof(szJson), "{\"video_decoder_info\":\"MediaCodec selected:[%s]\",\"mime\":\"%s\"}", szCodecName, mime);
    }
#endif

#if defined(__APPLE__) && !defined(__NO_VIDEOTOOLBOX__)
    if (videoDecoder == NULL && TYiOSVideoHardwareDecoder::HWDecodeSupported(mime)){
        videoDecoder = new TYiOSVideoHardwareDecoder(mime);

        snprintf(szJson, sizeof(szJson), "{\"video_decoder_info\":\"VideoToolBox selected.\", \"platform\":\"iOS\",\"mime\":\"%s\"}", mime);
    }
#endif
    
#if !defined(__NO_FFMPEG__)
    if (videoDecoder == NULL && TYFFMpegVideoDecoder::DecoderSupported(mime)){
        videoDecoder = new TYFFMpegVideoDecoder(mime);
        snprintf(szJson, sizeof(szJson), "{\"video_decoder_info\":\"FFmpeg selected.\", \"platform\":\"Android\",\"mime\":\"%s\"}", mime);
    }
#endif
    
#if !defined(__APPLE__) && !defined(__NO_OPENH264__)
    if (videoPacketInfo->codec_id == TY_AV_CODEC_VIDEO_H264){
        videoDecoder = new TYOpenH264Decoder;
        snprintf(szJson, sizeof(szJson), "{\"video_decoder_info\":\"openH264 selected.\", \"platform\":\"Android\",\"mime\":\"%s\"}", mime);
    }
#endif
    
    if (videoDecoder != NULL){
        videoDecoder->Init();
    }else{
        snprintf(szJson, sizeof(szJson), "{\"video_decoder_info\":\"No appropriate codec found.\",\"mime\":\"%s\"}", mime);
    }
    // TYLogManager::SendApmOnlineLog(TY_EVENT_IPC_CXX_SDK_EXTRA_DATA, szJson);

    return videoDecoder;
}

int TYVideoCodecSelector::DestroyDynamicVideoDecoder(TYBaseVideoDecoder **videoDecoder){
    int nExitCode = -1;
    TYBaseVideoDecoder *decoder = *videoDecoder;

    TY_PROCESS_ERROR(decoder);

    decoder->Uninit();
    TY_DELETE(decoder);

    nExitCode = 0;
Exit0:
    *videoDecoder = NULL;

    return nExitCode;
}
