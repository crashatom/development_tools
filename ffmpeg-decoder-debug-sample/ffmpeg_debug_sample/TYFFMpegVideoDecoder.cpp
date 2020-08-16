#include <sys/time.h>
#ifdef __APPLE__
#include <CoreVideo/CoreVideo.h>
#endif

#include "TYPublic.h"
#include "TYDebugOptions.h"
#include "TYFFMpegVideoDecoder.h"

TYFFMpegVideoDecoder::TYFFMpegVideoDecoder(const char *mime){
    m_bInit         = false;
    delegate        = NULL;
    m_pDecodedFrame = NULL;
    m_pYUVData      = NULL;
    m_pCodec        = NULL;
    m_pCodecContext = NULL;
    y               = NULL;
    u               = NULL;
    v               = NULL;
    strncpy(m_szMime, mime, sizeof(m_szMime));

    pthread_mutex_init(&m_codecMutex, NULL);

    Init(mime);
}

TYFFMpegVideoDecoder::~TYFFMpegVideoDecoder(){
    delegate = NULL;
    
    Uninit();

    pthread_mutex_destroy(&m_codecMutex);
}

bool TYFFMpegVideoDecoder::DecoderSupported(const char *mime){
    return strcasecmp(mime, "video/avc") == 0 || strcasecmp(mime, "video/hevc") == 0;
}

int TYFFMpegVideoDecoder::Init(const char *mime){
    int nExitCode = -1;
    int nRetCode  = -1;

    pthread_mutex_lock(&m_codecMutex);
    avcodec_register_all();
    av_register_all();

    if (NULL == m_pCodec){
        if (strcasecmp(mime, "video/avc") == 0){
            m_pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
        }else if (strcasecmp(mime, "video/hevc") == 0){
            m_pCodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
        }
    }
    m_pCodecContext = avcodec_alloc_context3(m_pCodec);
    
    TY_LOG_PROCESS_ERROR(m_pCodecContext != NULL, "ERROR: avcodec_alloc_context3 failed.\n");
    
    m_pCodecContext->time_base.num  = 1;
    m_pCodecContext->frame_number   = 1;                        // 每包一个视频帧
    m_pCodecContext->codec_type     = AVMEDIA_TYPE_VIDEO;
    m_pCodecContext->bit_rate       = 400000;                   // 比特率
    m_pCodecContext->time_base.den  = 15;                       // 帧率

    nRetCode = avcodec_open2(m_pCodecContext, m_pCodec, NULL);
    if (nRetCode < 0){
        char str[AV_ERROR_MAX_STRING_SIZE];
        memset(str, 0, sizeof(str));
        av_strerror(nRetCode, str, sizeof(str));
    }
    TY_LOG_PROCESS_ERROR(nRetCode >= 0, "ERROR: avcodec_open2 failed.\n");
    
    m_pDecodedFrame = av_frame_alloc();
    TY_PROCESS_ERROR(m_pDecodedFrame != NULL);
    
    m_bInit = true;
    nExitCode = 0;
Exit0:
    pthread_mutex_unlock(&m_codecMutex);

    return nExitCode;
}

int TYFFMpegVideoDecoder::Uninit(){
    // 如果没有初始化，直接结束
    TY_PROCESS_SUCCESS(false == m_bInit);
    
    m_bInit = false;
    
    pthread_mutex_lock(&m_codecMutex);
    
    if (m_pDecodedFrame != NULL)
    {
        av_frame_free(&m_pDecodedFrame);
        m_pDecodedFrame = NULL;
    }
    
    if (NULL != m_pCodecContext)
    {
        avcodec_close(m_pCodecContext);
        av_free(m_pCodecContext);
        m_pCodecContext = NULL;
    }
    pthread_mutex_unlock(&m_codecMutex);

    TY_DELETE(y);
    TY_DELETE(u);
    TY_DELETE(v);
    TY_DELETE(m_pYUVData);
    
Exit1:
    
    return 0;
}

int TYFFMpegVideoDecoder::DecodeNetStreamWithData(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo, void *userdata){
    AVPacket packet      = {0};                             // 必须初始化，否则avcodec_decode_video2可能崩溃
    struct timeval start = {0};
    struct timeval end   = {0};
    TY_PROCESS_ERROR(m_bInit);
    TY_PROCESS_ERROR(m_pDecodedFrame != NULL);
    TY_PROCESS_ERROR(videoPacketInfo->data != NULL);
    TY_PROCESS_ERROR(m_pCodecContext != NULL);
    
    pthread_mutex_lock(&m_codecMutex);

    packet.data = videoPacketInfo->data;                  // 这里填入一个指向完整H264数据帧的指针
    packet.size = videoPacketInfo->nDataSize;                   // 这个填入H264数据帧的大小
    GetSeiInfo(videoPacketInfo,packet.data,packet.size);

    gettimeofday( &start, NULL );

    avcodec_send_packet(m_pCodecContext, &packet);
    while(avcodec_receive_frame(m_pCodecContext, m_pDecodedFrame) >= 0) {
        gettimeofday( &end, NULL );

#if DEBUG_SHOW_VIDEO_FRAME_DECODE_TIME
        long timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;

        LOGI("DEBUG [%s]: ffmpeg decode time cost:%ldms\n", DEBUG_SHOW_VIDEO_FRAME_DECODE_TIME_TAG, timeuse / 1000);
#endif
        m_nWidth = m_pCodecContext->width;
        m_nHeight = m_pCodecContext->height;

        m_nYUVLength = m_pDecodedFrame->height * m_pDecodedFrame->width * 3 / 2;
        if (m_nYUVLength > 0 && NULL == m_pYUVData){
            m_pYUVData = new uint8_t[m_nYUVLength];
            TY_PROCESS_ERROR(m_pYUVData != NULL);
        }

#ifdef __APPLE__
        CVPixelBufferRef pixel_buffer = NULL;
        CFDictionaryRef empty = NULL;
        CFMutableDictionaryRef attrs = NULL;
        
        empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
        CVReturn err = CVPixelBufferCreate(kCFAllocatorDefault, m_nWidth, m_nHeight, kCVPixelFormatType_420YpCbCr8BiPlanarFullRange, attrs, &pixel_buffer);
        
        if (pixel_buffer) {
            CVPixelBufferLockBaseAddress(pixel_buffer, 0);
            // for y
            int y_stride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixel_buffer, 0);
            uint8_t *yDestPlane = (uint8_t *)CVPixelBufferGetBaseAddressOfPlane(pixel_buffer, 0);
            uint8_t *srcy = m_pDecodedFrame->data[0];
            for (int i = 0; i < m_nHeight; i++) {
                memcpy(yDestPlane, srcy, m_nWidth);
                srcy += m_pDecodedFrame->linesize[0];
                yDestPlane += y_stride;
            }
            // for uv
            uint8_t *uvDestPlane = (uint8_t *)CVPixelBufferGetBaseAddressOfPlane(pixel_buffer, 1);
            int uv_stride = (int)CVPixelBufferGetBytesPerRowOfPlane(pixel_buffer, 1);
            uint8_t *srcu = m_pDecodedFrame->data[1] ;
            uint8_t *srcv = m_pDecodedFrame->data[2] ;
            for (int j = 0; j < m_nHeight/2; j++) {
                uint8_t *uv = uvDestPlane ;
                uint8_t *u = srcu;
                uint8_t *v = srcv;
                for (int i = 0; i < m_nWidth/2; i++) {
                    *uv++ = *u ++ ;
                    *uv++ = *v ++ ;
                }
                uvDestPlane += uv_stride ;
                srcu += m_pDecodedFrame->linesize[1] ;
                srcv += m_pDecodedFrame->linesize[2] ;
            }
            CVPixelBufferUnlockBaseAddress(pixel_buffer, 0);
        }
        CFRelease(attrs);
        CFRelease(empty);
        
        draw_frames_for_image(videoPacketInfo, pixel_buffer);
        if (delegate != NULL){
            delegate->onVideoSpecificDecodedDataCompleted(videoPacketInfo, pixel_buffer, userdata);
        }
//        CFRelease(pixel_buffer);
#endif // __APPLE__
        
#ifdef __ANDROID__
        if (NULL == y || NULL == u || NULL == v){
            y = new uint8_t[m_pDecodedFrame->width * m_pDecodedFrame->height];
            u = new uint8_t[m_pDecodedFrame->width * m_pDecodedFrame->height / 4];
            v = new uint8_t[m_pDecodedFrame->width * m_pDecodedFrame->height / 4];
        }
        TY_PROCESS_ERROR(y != NULL);
        TY_PROCESS_ERROR(u != NULL);
        TY_PROCESS_ERROR(v != NULL);
        
        for (int i = 0, index = 0, offset = 0; i < m_pDecodedFrame->height; i++, index += m_pDecodedFrame->width, offset += m_pDecodedFrame->linesize[0]){
            memcpy(y + index, m_pDecodedFrame->data[0] + offset, m_pDecodedFrame->width);
        }
        
        for (int i = 0, index = 0, offset = 0; i < m_pDecodedFrame->height / 2; i++, index += m_pDecodedFrame->width / 2, offset += m_pDecodedFrame->linesize[1]){
            memcpy(u + index, m_pDecodedFrame->data[1] + offset, m_pDecodedFrame->width / 2);
        }
        
        for (int i = 0, index = 0, offset = 0; i < m_pDecodedFrame->height / 2; i++, index += m_pDecodedFrame->width / 2, offset += m_pDecodedFrame->linesize[2]){
            memcpy(v + index, m_pDecodedFrame->data[2] + offset, m_pDecodedFrame->width / 2);
        }
        // draw frame for active object.
        draw_frames_for_image(videoPacketInfo, y, u, v , m_pDecodedFrame->width, m_pDecodedFrame->height) ;

        if (delegate != NULL){
            delegate->onVideoYUVDataCompleted(delegate,
                                              videoPacketInfo->data.get(),
                                              (int)videoPacketInfo->nDataSize,
                                              y, u, v,
                                              m_pDecodedFrame->key_frame,
                                              videoPacketInfo->nFrameRate,
                                              m_pDecodedFrame->width,
                                              m_pDecodedFrame->height,
                                              videoPacketInfo->lldCustomTimestamp,
                                              videoPacketInfo->nRtpTimeStamp,
                                              videoPacketInfo->Progress.nDuration,
                                              videoPacketInfo->Progress.nProgress,
                                              videoPacketInfo->nRequestId,
                                              videoPacketInfo->sei,
                                              videoPacketInfo->nSeiSize,
                                              userdata);
        
//        if (delegate != NULL){
//            delegate->didFFmpegH264DecodedVideoDataCompleted(delegate, videoPacketInfo->data.get(), \
//                                                             (int)videoPacketInfo->nDataSize, y, u, v, \
//                                                             m_pDecodedFrame->key_frame, videoPacketInfo->nFrameRate, \
//                                                             m_pDecodedFrame->width, m_pDecodedFrame->height, \
//                                                             videoPacketInfo->lldCustomTimestamp, \
//                                                             videoPacketInfo->nRtpTimeStamp, \
//                                                             videoPacketInfo->Progress.nDuration, \
//                                                             videoPacketInfo->Progress.nProgress, \
//                                                             videoPacketInfo->nRequestId,
//                                                             videoPacketInfo->sei,
//                                                             videoPacketInfo->nSeiSize,
//                                                             userdata);


#if DEBUG_WRITE_YUV_DATA_TO_FILE
            remove("/sdcard/yuv.data");
            static FILE *YUVFile = fopen("/sdcard/yuv.data", "a+");

            fwrite(m_pDecodedFrame->data[0], 1, m_nWidth * m_nHeight, YUVFile);
            fwrite(m_pDecodedFrame->data[1], 1, m_nWidth * m_nHeight / 4, YUVFile);
            fwrite(m_pDecodedFrame->data[2], 1, m_nWidth * m_nHeight / 4, YUVFile);

            fclose(YUVFile);
#endif
        }
#endif // __ANDROID__

        av_packet_unref(&packet);
    }
    pthread_mutex_unlock(&m_codecMutex);
Exit0:

    return 0;
}

