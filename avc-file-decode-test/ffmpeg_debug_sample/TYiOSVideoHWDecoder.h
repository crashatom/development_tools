//
//  TYTYiOSVideoHardwareDecoder.h
//  TYCameraSdk
//
//  Created by Jim on 2019/7/17.
//  Copyright © 2019年 Tuya. All rights reserved.
//

#ifndef __TY_IOS_VIDEO_HW_DECODER_H__
#define __TY_IOS_VIDEO_HW_DECODER_H__

#ifdef __APPLE__

#include <list>
#include <pthread.h>
#include <CoreVideo/CoreVideo.h>
#include <VideoToolbox/VideoToolbox.h>

#include "TYBaseVideoDecoder.h"

//char g_NALU_TypesStrings[][128] = {
//    "0: Unspecified (non-VCL)",
//    "1: Coded slice of a non-IDR picture (VCL)",                    // P frame
//    "2: Coded slice data partition A (VCL)",
//    "3: Coded slice data partition B (VCL)",
//    "4: Coded slice data partition C (VCL)",
//    "5: Coded slice of an IDR picture (VCL)",                       // I frame
//    "6: Supplemental enhancement information (SEI) (non-VCL)",      // SEI
//    "7: Sequence parameter set (non-VCL)",                          // SPS parameter
//    "8: Picture parameter set (non-VCL)",                           // PPS parameter
//    "9: Access unit delimiter (non-VCL)",
//    "10: End of sequence (non-VCL)",
//    "11: End of stream (non-VCL)",
//    "12: Filler data (non-VCL)",
//    "13: Sequence parameter set extension (non-VCL)",
//    "14: Prefix NAL unit (non-VCL)",
//    "15: Subset sequence parameter set (non-VCL)",
//    "16: Reserved (non-VCL)",
//    "17: Reserved (non-VCL)",
//    "18: Reserved (non-VCL)",
//    "19: Coded slice of an auxiliary coded picture without partitioning (non-VCL)",
//    "20: Coded slice extension (non-VCL)",
//    "21: Coded slice extension for depth view components (non-VCL)",
//    "22: Reserved (non-VCL)",
//    "23: Reserved (non-VCL)",
//    "24: STAP-A Single-time aggregation packet (non-VCL)",
//    "25: STAP-B Single-time aggregation packet (non-VCL)",
//    "26: MTAP16 Multi-time aggregation packet (non-VCL)",
//    "27: MTAP24 Multi-time aggregation packet (non-VCL)",
//    "28: FU-A Fragmentation unit (non-VCL)",
//    "29: FU-B Fragmentation unit (non-VCL)",
//    "30: Unspecified (non-VCL)",
//    "31: Unspecified (non-VCL)",
//};

enum HEVCNALUnitType{
    HEVC_NAL_TRAIL_N    = 0,
    HEVC_NAL_TRAIL_R    = 1,
    HEVC_NAL_TSA_N      = 2,
    HEVC_NAL_TSA_R      = 3,
    HEVC_NAL_STSA_N     = 4,
    HEVC_NAL_STSA_R     = 5,
    HEVC_NAL_RADL_N     = 6,
    HEVC_NAL_RADL_R     = 7,
    HEVC_NAL_RASL_N     = 8,
    HEVC_NAL_RASL_R     = 9,
    HEVC_NAL_VCL_N10    = 10,
    HEVC_NAL_VCL_R11    = 11,
    HEVC_NAL_VCL_N12    = 12,
    HEVC_NAL_VCL_R13    = 13,
    HEVC_NAL_VCL_N14    = 14,
    HEVC_NAL_VCL_R15    = 15,
    HEVC_NAL_BLA_W_LP   = 16,
    HEVC_NAL_BLA_W_RADL = 17,
    HEVC_NAL_BLA_N_LP   = 18,
    HEVC_NAL_IDR_W_RADL = 19,
    HEVC_NAL_IDR_N_LP   = 20,
    HEVC_NAL_CRA_NUT    = 21,
    HEVC_NAL_IRAP_VCL22 = 22,
    HEVC_NAL_IRAP_VCL23 = 23,
    HEVC_NAL_RSV_VCL24  = 24,
    HEVC_NAL_RSV_VCL25  = 25,
    HEVC_NAL_RSV_VCL26  = 26,
    HEVC_NAL_RSV_VCL27  = 27,
    HEVC_NAL_RSV_VCL28  = 28,
    HEVC_NAL_RSV_VCL29  = 29,
    HEVC_NAL_RSV_VCL30  = 30,
    HEVC_NAL_RSV_VCL31  = 31,
    HEVC_NAL_VPS        = 32,
    HEVC_NAL_SPS        = 33,
    HEVC_NAL_PPS        = 34,
    HEVC_NAL_AUD        = 35,
    HEVC_NAL_EOS_NUT    = 36,
    HEVC_NAL_EOB_NUT    = 37,
    HEVC_NAL_FD_NUT     = 38,
    HEVC_NAL_SEI_PREFIX = 39,
    HEVC_NAL_SEI_SUFFIX = 40,
};

class TYiOSVideoHardwareDecoder: public TYBaseVideoDecoder{
public:
    TYiOSVideoHardwareDecoder(const char *mime);
    ~TYiOSVideoHardwareDecoder();
    
public:
    void CreateDecompressionSessionFromNetStream(CMFormatDescriptionRef formatDescription);
    uint8_t *GetH264NaluFromHead(uint8_t *pInData, long nInSize, int &nNaluSizeContainsLeadingCode, int &nNaluType, int &nLeadingCodeSize);
    uint8_t *GetHEVCNaluFromHead(uint8_t *pInData, long nInSize, int &nNaluSizeContainsLeadingCode, int &nNaluType, int &nLeadingCodeSize);
    
    int DecodeNetStreamWithData(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata);
    int DecodeNetStreamWithH264Format(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata);
    int DecodeNetStreamWithHEVCFormat(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata);
    static bool HWDecodeSupported(const char *mime);
    
    static void didDecompress(void *decompressionOutputRefCon,
                              void *sourceFrameRefCon,
                              OSStatus status,
                              VTDecodeInfoFlags infoFlags,
                              CVImageBufferRef imageBuffer,
                              CMTime presentationTimeStamp,
                              CMTime presentationDuration);
    void didDecompressimpl(void *sourceFrameRefCon,
                              OSStatus status,
                              VTDecodeInfoFlags infoFlags,
                              CVImageBufferRef imageBuffer,
                              CMTime presentationTimeStamp,
                              CMTime presentationDuration);
    int Init();
    int Uninit();
    void EnableIVA(bool enable);
public:
    VTDecompressionSessionRef m_decompressionSession;
    pthread_mutex_t m_Mutex;
    void *m_userdata;
    
    CMFormatDescriptionRef formatDescription;
    
private:
    char m_szMime[64];
};

#endif  // __APPLE__

#endif  // __TY_IOS_VIDEO_HW_DECODER_H__
