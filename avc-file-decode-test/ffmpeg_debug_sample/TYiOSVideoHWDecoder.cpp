//
//  TYiOSHardwareDecoder.cpp
//  TYCameraSdk
//
//  Created by Jim on 2018/3/14.
//  Copyright © 2018年 Tuya. All rights reserved.
//

#ifdef __APPLE__

#include "TYPublic.h"
#include "TYiOSVideoHWDecoder.h"

#include <stdio.h>
#include <CoreFoundation/CoreFoundation.h>

const uint8_t nalu_leading_code[2][5] = {
    {4, 0x00, 0x00, 0x00, 0x01},    // 4 byte leading code.
    {3, 0x00, 0x00, 0x01, 0x00},    // 3 byte leading code.
};

TYiOSVideoHardwareDecoder::TYiOSVideoHardwareDecoder(const char *mime){
    delegate = NULL;
    
    formatDescription = NULL;
    m_decompressionSession = NULL;
    
    pthread_mutex_init(&m_Mutex, NULL);
    
    strncpy(m_szMime, mime, sizeof(m_szMime));
}

TYiOSVideoHardwareDecoder::~TYiOSVideoHardwareDecoder(){
    pthread_mutex_lock(&m_Mutex);
    delegate = NULL;
    
    if (m_decompressionSession != NULL){
        VTDecompressionSessionWaitForAsynchronousFrames(m_decompressionSession);
        VTDecompressionSessionInvalidate(m_decompressionSession);
        
        CFRelease(m_decompressionSession);
        m_decompressionSession = NULL;
    }
    
    if (formatDescription){
        CFRelease(formatDescription);
        formatDescription = NULL;
    }
    
    pthread_mutex_unlock(&m_Mutex);
}

bool TYiOSVideoHardwareDecoder::HWDecodeSupported(const char *mime){
    // return false;
    
    int majorVersion = 0;
    char szVersion[128] = {0};
    
    // check H.264: os version must >= 8.0
    if (strcasecmp(mime, "video/avc") == 0){
         if (__builtin_available(iOS 8.0, *)){
             return true;
         }
        return majorVersion >= 8;
    }
    
    // check HEVC: os version must >= 11.0
    if (strcasecmp(mime, "video/hevc") == 0){
        if (__builtin_available(iOS 11.0, *)){
            return true;
        }
        return majorVersion >= 11;
    }
    
    return false;
}

int TYiOSVideoHardwareDecoder::Init(){
    return 0;
}

int TYiOSVideoHardwareDecoder::Uninit(){
    return 0;
}

// 创建解码session
void TYiOSVideoHardwareDecoder::CreateDecompressionSessionFromNetStream(CMFormatDescriptionRef formatDescription){
    OSStatus status = noErr;
    VTDecompressionOutputCallbackRecord callbackRecord;
    CFDictionaryRef attrs = NULL;
    const void *keys[] = { kCVPixelBufferPixelFormatTypeKey };
    
    // kCVPixelFormatType_420YpCbCr8Planar              YUV420
    // kCVPixelFormatType_420YpCbCr8BiPlanarFullRange   NV12
    uint32_t v = kCVPixelFormatType_420YpCbCr8BiPlanarFullRange;
    const void *values[] = { CFNumberCreate(NULL, kCFNumberSInt32Type, &v) };
    attrs = CFDictionaryCreate(NULL, keys, values, 1, NULL, NULL);
    
    m_decompressionSession = NULL;
    callbackRecord.decompressionOutputCallback = didDecompress;
    callbackRecord.decompressionOutputRefCon = this;
    
    status = VTDecompressionSessionCreate(NULL, formatDescription, NULL, attrs, &callbackRecord, &m_decompressionSession);
    
    TY_LOG_PROCESS_ERROR(noErr == status, "ERROR: Video decompression session create failed.\n");
    
Exit0:
    
    CFRelease(attrs);
    
    return ;
}

uint8_t *TYiOSVideoHardwareDecoder::GetH264NaluFromHead(uint8_t *pInData, long nInSize, int &nNaluSizeContainsLeadingCode, int &nNaluType, int &nLeadingCodeSize){
    if (NULL == pInData) return NULL;
    
    nNaluType = 0;
    nLeadingCodeSize = 0;
    nNaluSizeContainsLeadingCode = 0;
    int leading_code_types = sizeof(nalu_leading_code) / sizeof(nalu_leading_code[0]);
    
    // O(N)
    for (int i = 0; i < nInSize; i++){
        for (int k = 0; k < leading_code_types; k++){
            if (i + nalu_leading_code[k][0] > nInSize){
                break;
            }
            
            if (0 == memcmp((void *)(pInData + i), (void *)(&nalu_leading_code[k][1]), (size_t)nalu_leading_code[k][0])){
                nNaluType = (pInData + i + nalu_leading_code[k][0])[0];
                
                for (int j = i + nalu_leading_code[k][0]; j < nInSize; j++){
                    for (int o = 0; o < leading_code_types; o++){
                        if (j + nalu_leading_code[o][0] <= nInSize && 0 == memcmp((void *)(pInData + j), (void *)(&nalu_leading_code[o][1]), (size_t)nalu_leading_code[o][0])){
                            nNaluSizeContainsLeadingCode = j - i;
                            nLeadingCodeSize = nalu_leading_code[k][0];
                            
                            return pInData + i;
                        }
                    }
                }
                
                nNaluSizeContainsLeadingCode = (int)nInSize - i;
                nLeadingCodeSize = nalu_leading_code[k][0];
                
                return pInData + i;
            }
        }
    }
    
    return pInData + nInSize;
}

uint8_t *TYiOSVideoHardwareDecoder::GetHEVCNaluFromHead(uint8_t *pInData, long nInSize, int &nNaluSizeContainsLeadingCode, int &nNaluType, int &nLeadingCodeSize){
    if (NULL == pInData) return NULL;
    
    nNaluType = 0;
    nLeadingCodeSize = 0;
    nNaluSizeContainsLeadingCode = 0;
    int leading_code_types = sizeof(nalu_leading_code) / sizeof(nalu_leading_code[0]);
    
    // O(N)
    for (int i = 0; i < nInSize; i++){
        for (int k = 0; k < leading_code_types; k++){
            if (i + nalu_leading_code[k][0] > nInSize){
                break;
            }
            
            if (0 == memcmp((void *)(pInData + i), (void *)(&nalu_leading_code[k][1]), (size_t)nalu_leading_code[k][0])){
                nNaluType = (pInData + i + nalu_leading_code[k][0])[0];
                
                for (int j = i + nalu_leading_code[k][0] + 1; j < nInSize; j++){
                    for (int o = 0; o < leading_code_types; o++){
                        if (j + nalu_leading_code[o][0] <= nInSize && 0 == memcmp((void *)(pInData + j), (void *)(&nalu_leading_code[o][1]), (size_t)nalu_leading_code[o][0])){
                            nNaluSizeContainsLeadingCode = j - i;
                            nLeadingCodeSize = nalu_leading_code[k][0];
                            
                            return pInData + i;
                        }
                    }
                }
                
                nNaluSizeContainsLeadingCode = (int)nInSize - i;
                nLeadingCodeSize = nalu_leading_code[k][0];
                
                return pInData + i;
            }
        }
    }
    
    return pInData + nInSize;
}

int TYiOSVideoHardwareDecoder::DecodeNetStreamWithData(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata){
    int nRetCode = -1;
    
    switch (videoPacketInfo->codec_id) {
        case TY_AV_CODEC_VIDEO_H264:
            nRetCode = DecodeNetStreamWithH264Format(videoPacketInfo, userdata);
            break;
        case TY_AV_CODEC_VIDEO_H265:
            nRetCode = DecodeNetStreamWithHEVCFormat(videoPacketInfo, userdata);
            break;
        default:
            LOGI("TYCameraSDK [ERROR]: unsupported video format.\n");
            break;
    }
    
    return nRetCode;
}

int parse_nalu_sei_(unsigned char * data, int size, const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo){
    unsigned char * sei = data;
    int sei_type = *sei++ ;
    int sei_size = *sei++ ;
    
    if (sei_size > UUID_SIZE && sei_size <= (data + size - sei) && sei_type == 5)       // we do seitype=5 only
    {
        char *uuid = new char [UUID_SIZE+1];
        memcpy(uuid, sei, UUID_SIZE);
        sei += UUID_SIZE;
        sei_size -= UUID_SIZE ;
        char *seiInfo = new char[sei_size+1];
        memcpy(seiInfo, sei, sei_size);
        videoPacketInfo->uuid = uuid ;
        videoPacketInfo->nUuidSize = UUID_SIZE;
        videoPacketInfo->sei = seiInfo ;
        videoPacketInfo->nSeiSize = sei_size ;
#if DEBUG_IVA_SEI_INFO
        LOGI("parse_nalu_sei sei :%s  \n",(char*)seiInfo);
#endif
    }else{
#if DEBUG_IVA_SEI_INFO
        LOGI("parse_nalu_sei no sei info \n");
#endif
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1. SPS、PPS nalu是4字节起始码；
// 2. Access Unit的首个nalu是4字节起始码（参见7.4.1.2.3）。
//       SPS            （4字节头）
//       PPS            （4字节头）
//       SEI            （4字节头）
//       I0(slice0)     （4字节头）
//       I0(slice1)     （3字节头）
//       P1(slice0)     （4字节头）
//       P1(slice1)     （3字节头）
//       P2(slice0)     （4字节头）
//       P2(slice1)     （3字节头）
// I0(slice0)是序列第一帧（I帧）的第一个slice，是当前Access Unit的首个nalu，所以是4字节头。而I0(slice1)表示第一帧的第二个slice，所以是3字节头
// P1(slice0) 、P1(slice1)同理。
//
//
// 总结：
// 1 附录 B字节流在一个byte_stream_nal_unit的前后可能出现若干个0x00，仅用作填充之用
// 2 4字节头只出现在SPS、PPS和7.4.1.2.3规定的Access Unit的首个nalu，其余情况都是3字节头
//
//
//
// 一共有两种起始码：3字节的0x000001和4字节的0x00000001
// 3字节的0x000001只有一种场合下使用，就是一个完整的帧被编为多个slice的时候，包含这些slice的nalu使用3字节起始码。其余场合都是4字节的。
// http://stackoverflow.com/questions/29525000/how-to-use-videotoolbox-to-decompress-h-264-video-stream/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TYiOSVideoHardwareDecoder::DecodeNetStreamWithH264Format(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata){
    pthread_mutex_lock(&m_Mutex);
    VTDecodeFrameFlags flags            = 0;        // 0:同步; kVTDecodeFrame_EnableAsynchronousDecompression:异步
    VTDecodeInfoFlags flagOut           = 0;
    OSStatus status                     = 0;
    int nExitCode                       = -1;
    int nNaluType                       = 0;
    unsigned long uDataRemainLen        = 0;
    int nLeadingCodeSize                = 0;
    int nNaluSizeContainsLeadingCode    = 0;
    unsigned long uBlockLength          = 0;
    int nSpsSize                        = 0;
    int nPpsSize                        = 0;
    uint8_t *spscopy                    = NULL;
    uint8_t *ppscopy                    = NULL;
    bool bIsIFrame                      = false;
    const uint8_t *sps                  = NULL;
    const uint8_t *pps                  = NULL;
    uint8_t *pBlock                     = NULL;
    CMBlockBufferRef blockBufferRef     = NULL;
    CMSampleBufferRef sampleBufferRef   = NULL;
    uint8_t *data                       = NULL;
    uint8_t *pData                      = videoPacketInfo->data;
    unsigned long uLen                  = videoPacketInfo->nDataSize;
    
    data = pData;
    uDataRemainLen = uLen;
    GetSeiInfo(videoPacketInfo,videoPacketInfo->data ,videoPacketInfo->nDataSize);
    while ((data = GetH264NaluFromHead(data, uDataRemainLen, nNaluSizeContainsLeadingCode, nNaluType, nLeadingCodeSize)) != NULL){
        int type = nNaluType & 0x1F;
        
        // Ignore SEI.
        if (6 == type){
            uDataRemainLen = uLen - (data + nNaluSizeContainsLeadingCode - pData);
            data += nNaluSizeContainsLeadingCode;
            // uDataRemainLen = uLen - (data + nNaluSizeContainsLeadingCode - pData);
            // unsigned char *sei = (unsigned char *)&data[nLeadingCodeSize+1] ;
            // parse_nalu_sei_(sei,(int)uDataRemainLen-1,videoPacketInfo);
            // data += nNaluSizeContainsLeadingCode;
            continue;
        }
        
        if (7 == type){
            nSpsSize = nNaluSizeContainsLeadingCode - nLeadingCodeSize;
            sps = &data[nLeadingCodeSize];
            
            TY_DELETE_ARRAY(spscopy);
            spscopy = new uint8_t[nSpsSize];
            TY_PROCESS_ERROR(spscopy != NULL);
            
            memcpy(spscopy, sps, nSpsSize);
        }else if (8 == type){
            nPpsSize = nNaluSizeContainsLeadingCode - nLeadingCodeSize;
            pps = &data[nLeadingCodeSize];
            
            TY_DELETE_ARRAY(ppscopy);
            ppscopy = new uint8_t[nPpsSize];
            TY_PROCESS_ERROR(ppscopy != NULL);
            
            memcpy(ppscopy, pps, nPpsSize);
        }else if (1 == type || 5 == type){
            bIsIFrame = (type == 5);
            
            uBlockLength = nNaluSizeContainsLeadingCode - nLeadingCodeSize + 4;
            pBlock = new uint8_t[uBlockLength];
            TY_PROCESS_ERROR(pBlock != NULL);
            
            int nAddtionalByte = 4 - nLeadingCodeSize;
            
            memcpy((uint8_t *)pBlock + nAddtionalByte, data, nNaluSizeContainsLeadingCode);
            
            uint32_t nDataLength32 = htonl(uBlockLength - nLeadingCodeSize);
            memcpy(pBlock, &nDataLength32, sizeof(uint32_t));
            
            status = CMBlockBufferCreateWithMemoryBlock(NULL, pBlock,
                                                        uBlockLength,
                                                        kCFAllocatorNull,
                                                        NULL, 0,
                                                        uBlockLength, 0,
                                                        &blockBufferRef);
            TY_LOG_PROCESS_ERROR(kCMBlockBufferNoErr == status, "ERROR: BlockBufferCreation failed.\n");
            
            break;
        }
        
        if (NULL != spscopy && NULL != ppscopy){
            uint8_t *pParamSetPointers[2] = {spscopy, ppscopy};
            size_t paramSetSizes[] = {static_cast<size_t>(nSpsSize), static_cast<size_t>(nPpsSize)};
            
            if (formatDescription){
                CFRelease(formatDescription);
                formatDescription = NULL;
            }
            
            status = CMVideoFormatDescriptionCreateFromH264ParameterSets(kCFAllocatorDefault, 2, (const uint8_t * const *)pParamSetPointers, paramSetSizes, 4, &formatDescription);
            TY_LOG_PROCESS_ERROR(noErr == status, "ERROR: CMVideoFormatDescriptionCreateFromH264ParameterSets failed.\n");
            
            if (NULL == m_decompressionSession){
                CreateDecompressionSessionFromNetStream(formatDescription);
            }else{
                if (VTDecompressionSessionCanAcceptFormatDescription(m_decompressionSession, formatDescription) == false){
                    VTDecompressionSessionWaitForAsynchronousFrames(m_decompressionSession);
                    VTDecompressionSessionInvalidate(m_decompressionSession);
                    
                    CFRelease(m_decompressionSession);
                    m_decompressionSession = NULL;
                    
                    CreateDecompressionSessionFromNetStream(formatDescription);
                }
            }
        }
        
        uDataRemainLen = uLen - (data + nNaluSizeContainsLeadingCode - pData);
        data += nNaluSizeContainsLeadingCode;
        
        if (uDataRemainLen <= 0)
            break;
    }
    
    TY_PROCESS_SUCCESS(uBlockLength <= 0);
    TY_PROCESS_ERROR(m_decompressionSession != NULL);
    
    if (noErr == status){
        CMSampleTimingInfo timming;
        timming.decodeTimeStamp = kCMTimeInvalid;
        timming.presentationTimeStamp = CMTimeMake(videoPacketInfo->lldCustomTimestamp, 90000);
        timming.duration = CMTimeMake(3000, 90000);
        
        const size_t sampleSize = uBlockLength;
        int sample_count = 1;
        
        status = CMSampleBufferCreateReady(kCFAllocatorDefault,     // CFAllocatorRef allocator
                                           blockBufferRef,          // CMBlockBufferRef dataBuffer
                                           formatDescription,       // CMFormatDescriptionRef formatDescription
                                           sample_count,            // CMItemCount numSamples
                                           1,                       // CMItemCount numSampleTimingEntries
                                           &timming,                // const CMSampleTimingInfo *sampleTimingArray
                                           sample_count,            // CMItemCount numSampleSizeEntries
                                           &sampleSize,             // const size_t *sampleSizeArray
                                           &sampleBufferRef);       // CMSampleBufferRef *sBufOut
        
        
        TY_LOG_PROCESS_ERROR(noErr == status, "ERROR: sampleBufferCreate failed.\n");
    }
    
    // CFArrayRef attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBufferRef, true);
    // CFMutableDictionaryRef dict = (CFMutableDictionaryRef)CFArrayGetValueAtIndex(attachments, 0);
    // CFDictionarySetValue(dict, kCMSampleAttachmentKey_DisplayImmediately, kCFBooleanTrue);
    
    m_userdata = userdata;
    
    VTDecompressionSessionDecodeFrame(m_decompressionSession, sampleBufferRef, flags, (void *)&videoPacketInfo, &flagOut);
    
Exit1:
    nExitCode = 0;
Exit0:
    TY_DELETE_ARRAY(spscopy);
    TY_DELETE_ARRAY(ppscopy);
    TY_DELETE_ARRAY(pBlock);
    
    if (sampleBufferRef){
        CMSampleBufferInvalidate(sampleBufferRef);
        CFRelease(sampleBufferRef);
        sampleBufferRef = NULL;
    }
    if (blockBufferRef)
        CFRelease(blockBufferRef);
    
    pthread_mutex_unlock(&m_Mutex);
    
    return nExitCode;
}

int TYiOSVideoHardwareDecoder::DecodeNetStreamWithHEVCFormat(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata){
    pthread_mutex_lock(&m_Mutex);
    VTDecodeFrameFlags flags            = 0;        // 0:同步; kVTDecodeFrame_EnableAsynchronousDecompression:异步
    VTDecodeInfoFlags flagOut           = 0;
    OSStatus status                     = 0;
    int nExitCode                       = -1;
    int nNaluType                       = 0;
    unsigned long uDataRemainLen        = 0;
    int nLeadingCodeSize                = 0;
    int nNaluSizeContainsLeadingCode    = 0;
    unsigned long uBlockLength          = 0;
    int nVpsSize                        = 0;
    int nSpsSize                        = 0;
    int nPpsSize                        = 0;
    uint8_t *vpscopy                    = NULL;
    uint8_t *spscopy                    = NULL;
    uint8_t *ppscopy                    = NULL;
    bool bIsIFrame                      = false;
    const uint8_t *vps                  = NULL;
    const uint8_t *sps                  = NULL;
    const uint8_t *pps                  = NULL;
    uint8_t *pBlock                     = NULL;
    CMBlockBufferRef blockBufferRef     = NULL;
    CMSampleBufferRef sampleBufferRef   = NULL;
    uint8_t *data                       = NULL;
    uint8_t *pData                      = videoPacketInfo->data;
    unsigned long uLen                  = videoPacketInfo->nDataSize;
    
    data = pData;
    uDataRemainLen = uLen;
    
    while ((data = GetHEVCNaluFromHead(data, uDataRemainLen, nNaluSizeContainsLeadingCode, nNaluType, nLeadingCodeSize)) != NULL){
        int type = (nNaluType & 0x7E) >> 1;
        
        if (HEVC_NAL_TRAIL_N == type){
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: B Slice\n", DEBUG_HEVC_RTP_DEBUG_TAG);
#endif
        }else if (HEVC_NAL_SEI_PREFIX == type){
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: SEI slice\n", DEBUG_HEVC_RTP_DEBUG_TAG);
#endif
        }else if (HEVC_NAL_VPS == type){
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: VPS slice\n", DEBUG_HEVC_RTP_DEBUG_TAG);
#endif
            
            nVpsSize = nNaluSizeContainsLeadingCode - nLeadingCodeSize;
            vps = &data[nLeadingCodeSize];
            
            TY_DELETE_ARRAY(vpscopy);
            vpscopy = new uint8_t[nVpsSize];
            TY_PROCESS_ERROR(vpscopy != NULL);
            
            memcpy(vpscopy, vps, nVpsSize);
        }else if (HEVC_NAL_SPS == type){
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: SPS slice\n", DEBUG_HEVC_RTP_DEBUG_TAG);
#endif
            
            nSpsSize = nNaluSizeContainsLeadingCode - nLeadingCodeSize;
            sps = &data[nLeadingCodeSize];
            
            TY_DELETE_ARRAY(spscopy);
            spscopy = new uint8_t[nSpsSize];
            TY_PROCESS_ERROR(spscopy != NULL);
            
            memcpy(spscopy, sps, nSpsSize);
        }else if (HEVC_NAL_PPS == type){
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: PPS slice\n", DEBUG_HEVC_RTP_DEBUG_TAG);
#endif
            
            nPpsSize = nNaluSizeContainsLeadingCode - nLeadingCodeSize;
            pps = &data[nLeadingCodeSize];
            
            TY_DELETE_ARRAY(ppscopy);
            ppscopy = new uint8_t[nPpsSize];
            TY_PROCESS_ERROR(ppscopy != NULL);
            
            memcpy(ppscopy, pps, nPpsSize);
        }else if (HEVC_NAL_TRAIL_R == type || HEVC_NAL_IDR_W_RADL == type) {
#if DEBUG_HEVC_RTP_DEBUG
            LOGI("DEBUG [%s]: %s\n", DEBUG_HEVC_RTP_DEBUG_TAG, (HEVC_NAL_IDR_W_RADL == type) ? "HEVC_NAL_IDR_W_RADL" : "HEVC_NAL_TRAIL_R");
#endif
            
            bIsIFrame = (HEVC_NAL_IDR_W_RADL == type);
            
            uBlockLength = nNaluSizeContainsLeadingCode - nLeadingCodeSize + 4;
            pBlock = new uint8_t[uBlockLength];
            TY_PROCESS_ERROR(pBlock != NULL);
            
            int nAddtionalByte = 4 - nLeadingCodeSize;
            
            memcpy((uint8_t *)pBlock + nAddtionalByte, data, nNaluSizeContainsLeadingCode);
            
            uint32_t nDataLength32 = htonl(uBlockLength - nLeadingCodeSize);
            memcpy(pBlock, &nDataLength32, sizeof(uint32_t));
            
            status = CMBlockBufferCreateWithMemoryBlock(NULL, pBlock,
                                                        uBlockLength,
                                                        kCFAllocatorNull,
                                                        NULL, 0,
                                                        uBlockLength, 0,
                                                        &blockBufferRef);
            TY_LOG_PROCESS_ERROR(kCMBlockBufferNoErr == status, "ERROR: BlockBufferCreation failed.\n");
            
            break;
        }
        
        if (NULL != vpscopy && NULL != spscopy && NULL != ppscopy){
            uint8_t *pParamSetPointers[3] = {vpscopy, spscopy, ppscopy};
            size_t paramSetSizes[] = {static_cast<size_t>(nVpsSize), static_cast<size_t>(nSpsSize), static_cast<size_t>(nPpsSize)};
            
            if (formatDescription){
                CFRelease(formatDescription);
                formatDescription = NULL;
            }
            
            // 系统版本检查
            int majorVersion = 0;
            char szVersion[128] = {0};
            // TuyaSmartIPC::CXX::TYSmartCameraSDK::GetiOSSystemVersion(szVersion, sizeof(szVersion));
            sscanf(szVersion, "%d.", &majorVersion);
            
            if (majorVersion >= 11) {
                status = CMVideoFormatDescriptionCreateFromHEVCParameterSets(kCFAllocatorDefault, 3, pParamSetPointers, paramSetSizes, 4, NULL, &formatDescription);
            } else {
                LOGI("TYCameraSDK [ERROR]: hevc not support, trigger by builtin check.\n");
                
                TY_PROCESS_ERROR(false);
            }
            
            TY_LOG_PROCESS_ERROR(noErr == status, "ERROR: CMVideoFormatDescriptionCreateFromH264ParameterSets failed.\n");
            
            if (NULL == m_decompressionSession){
                CreateDecompressionSessionFromNetStream(formatDescription);
            }else{
                if (VTDecompressionSessionCanAcceptFormatDescription(m_decompressionSession, formatDescription) == false){
                    VTDecompressionSessionWaitForAsynchronousFrames(m_decompressionSession);
                    VTDecompressionSessionInvalidate(m_decompressionSession);
                    
                    CFRelease(m_decompressionSession);
                    m_decompressionSession = NULL;
                    
                    CreateDecompressionSessionFromNetStream(formatDescription);
                }
            }
        }
        
        uDataRemainLen = uLen - (data + nNaluSizeContainsLeadingCode - pData);
        data += nNaluSizeContainsLeadingCode;
        
        if (uDataRemainLen <= 0)
            break;
    }
    
    TY_PROCESS_SUCCESS(uBlockLength <= 0);
    TY_PROCESS_ERROR(m_decompressionSession != NULL);
    
    if (noErr == status){
        CMSampleTimingInfo timming;
        timming.decodeTimeStamp = kCMTimeInvalid;
        timming.presentationTimeStamp = CMTimeMake(videoPacketInfo->lldCustomTimestamp, 90000);
        timming.duration = CMTimeMake(3000, 90000);
        
        const size_t sampleSize = uBlockLength;
        int sample_count = 1;
        
        status = CMSampleBufferCreateReady(kCFAllocatorDefault,     // CFAllocatorRef allocator
                                           blockBufferRef,          // CMBlockBufferRef dataBuffer
                                           formatDescription,       // CMFormatDescriptionRef formatDescription
                                           sample_count,            // CMItemCount numSamples
                                           1,                       // CMItemCount numSampleTimingEntries
                                           &timming,                // const CMSampleTimingInfo *sampleTimingArray
                                           sample_count,            // CMItemCount numSampleSizeEntries
                                           &sampleSize,             // const size_t *sampleSizeArray
                                           &sampleBufferRef);       // CMSampleBufferRef *sBufOut
        
        
        TY_LOG_PROCESS_ERROR(noErr == status, "ERROR: sampleBufferCreate failed.\n");
    }
    
    // CFArrayRef attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBufferRef, true);
    // CFMutableDictionaryRef dict = (CFMutableDictionaryRef)CFArrayGetValueAtIndex(attachments, 0);
    // CFDictionarySetValue(dict, kCMSampleAttachmentKey_DisplayImmediately, kCFBooleanTrue);
    
    m_userdata = userdata;
    
    status = VTDecompressionSessionDecodeFrame(m_decompressionSession, sampleBufferRef, flags, (void *)&videoPacketInfo, &flagOut);
    
    if (status == kVTInvalidSessionErr){
        LOGI("TYCameraSDK [ERROR]: VTDecompressionSessionDecodeFrame failed, retcode = %d.\n", (int)status);
    }
    
Exit1:
    nExitCode = 0;
Exit0:
    TY_DELETE_ARRAY(spscopy);
    TY_DELETE_ARRAY(ppscopy);
    TY_DELETE_ARRAY(pBlock);
    
    if (sampleBufferRef){
        CMSampleBufferInvalidate(sampleBufferRef);
        CFRelease(sampleBufferRef);
        sampleBufferRef = NULL;
    }
    if (blockBufferRef)
        CFRelease(blockBufferRef);
    
    pthread_mutex_unlock(&m_Mutex);
    
    return nExitCode;
}

#pragma mark - VideoToolBox Decompress Frame CallBack
/**
 *  Description: This callback gets called everytime the decompresssion session decodes a frame
 *
 *  @param decompressionOutputRefCon decompressionOutputRefCon description
 *  @param sourceFrameRefCon         sourceFrameRefCon description
 *  @param status                    status description
 *  @param infoFlags                 infoFlags description
 *  @param imageBuffer               imageBuffer description
 *  @param presentationTimeStamp     presentationTimeStamp description
 *  @param presentationDuration      presentationDuration description
 */
void TYiOSVideoHardwareDecoder::didDecompress(void *decompressionOutputRefCon, void *sourceFrameRefCon, OSStatus status, VTDecodeInfoFlags infoFlags, CVImageBufferRef imageBuffer, CMTime presentationTimeStamp, CMTime presentationDuration) {
    TYiOSVideoHardwareDecoder *decoder = (TYiOSVideoHardwareDecoder *)decompressionOutputRefCon;
    
    decoder->didDecompressimpl( sourceFrameRefCon, status, infoFlags, imageBuffer, presentationTimeStamp, presentationDuration);
}

void TYiOSVideoHardwareDecoder::didDecompressimpl(
                       void *sourceFrameRefCon,
                       OSStatus status,
                       VTDecodeInfoFlags infoFlags,
                       CVImageBufferRef imageBuffer,
                       CMTime presentationTimeStamp,
                       CMTime presentationDuration){
    // Error code see VTErrors.h
    if (noErr != status){
        return ;
    }

    TY_PROCESS_ERROR(NULL != imageBuffer);
    {
        const std::shared_ptr<TYVideoPacketInfo> &info = *(const std::shared_ptr<TYVideoPacketInfo> *)sourceFrameRefCon;
        CVPixelBufferLockBaseAddress(imageBuffer, 0);
        
        // Get pixel buffer info
        int srcw = (int)CVPixelBufferGetWidth(imageBuffer);
        int srch = (int)CVPixelBufferGetHeight(imageBuffer);
        int srcystride = (int)CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
        int srcuvstride = (int)CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 1);
        
        unsigned char *srcy = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
        unsigned char *srcuv = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 1) ;
        
        // Copy the pixel buffer
        OSType type = CVPixelBufferGetPixelFormatType(imageBuffer);
        CVPixelBufferRef pixelBufferCopy = NULL;
        
        CFDictionaryRef empty;
        CFMutableDictionaryRef attrs;
        // Create empty IOSurface properties dictionary 创建GPU可直接访问内存型PixelBuffer
        empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        
        CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
        
        // 创建具有kCVPixelBufferIOSurfacePropertiesKey属性的CVPixelBufferRef实例renderTarget
        CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, srcw , srch, type, attrs, &pixelBufferCopy);
        if (status == kCVReturnSuccess){
            CVPixelBufferLockBaseAddress(pixelBufferCopy, 0);
            int dstw = (int)CVPixelBufferGetWidth(pixelBufferCopy);
            int dsth = (int)CVPixelBufferGetHeight(pixelBufferCopy);
            int dstystride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBufferCopy, 0);
            int dstuvstride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixelBufferCopy, 1);
            
            unsigned char *dsty = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBufferCopy, 0);
            unsigned char *dstu = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBufferCopy, 1);
            for (int i = 0; i < dsth; i++) {
                memcpy(dsty + i * dstystride, srcy + i * srcystride, dstw);
            }
            for (int i = 0; i < dsth / 2; i ++) {
                memcpy(dstu + i * dstuvstride, srcuv + i * srcuvstride, dstw);
            }
            CVPixelBufferUnlockBaseAddress(pixelBufferCopy, 0);
            draw_frames_for_image(info,pixelBufferCopy) ;
            
            if (NULL != delegate){
                delegate->onVideoSpecificDecodedDataCompleted(info, pixelBufferCopy, m_userdata);
            }
        }
        CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    }
    
Exit0:
    return ;
}

#endif // __APPLE__
