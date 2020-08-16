#include "TYUtils.h"
#include "TYBaseVideoDecoder.h"

TYBaseVideoDecoder::TYBaseVideoDecoder(){
    m_bIVAEnable = false ;
    activeObjs =  NULL ;
    nObjsNumbers = 0 ;
    m_bLastFrameArrive = false ;
}

TYBaseVideoDecoder::~TYBaseVideoDecoder(){
    if (activeObjs) {
        delete [] activeObjs ;
        activeObjs = NULL ;
    }
}

void TYBaseVideoDecoder::GetSeiInfo(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,uint8_t *stream,int size){
    unsigned char *seiData = NULL ;
    // find sei and parse it
    videoPacketInfo->uuid = NULL ;
    videoPacketInfo->nUuidSize = 0;
    videoPacketInfo->sei =  NULL ;
    videoPacketInfo->nSeiSize = 0 ;
    
    seiData = find_nalu_sei(stream,size);
    if(seiData){
        char *uuid = NULL ;
        int uuidSize = 0 ;
        char *sei = NULL ;
        int seiSize = 0 ;
        
        parse_nalu_sei(seiData, size - (seiData - stream) ,&uuid,uuidSize,&sei,seiSize) ;
        if(uuid != NULL && uuidSize > 0){
            videoPacketInfo->uuid = uuid ;
            videoPacketInfo->nUuidSize = uuidSize;
        }
        
        if(sei != NULL && seiSize > 0){
            videoPacketInfo->sei = sei ;
            videoPacketInfo->nSeiSize = seiSize ;
            
#if DEBUG_IVA_SEI_INFO
            LOGI("parse_nalu_sei result here sei:%p seiSize:%d .....\n", sei, seiSize);
#endif
        }
    }
}

void TYBaseVideoDecoder::draw_frames_for_image(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,uint8_t *y,uint8_t *u,uint8_t *v,int w,int h){
    if (videoPacketInfo->sei && videoPacketInfo->nSeiSize > 0) {
        nObjsNumbers = 0 ;
        if (activeObjs) {
            delete[] activeObjs ;
            activeObjs = NULL ;
        }
        get_point_pos_(videoPacketInfo->sei,videoPacketInfo->nSeiSize,&activeObjs,nObjsNumbers);
    }
    
    if (nObjsNumbers > 0 && activeObjs && m_bIVAEnable) {
        for (int i = 0; i < nObjsNumbers; i ++) {
            draw_one_frame_yuv420(y,u,v,w,h, activeObjs[i]);
        }
    }else{
        if(videoPacketInfo->sei){
            delete[] videoPacketInfo->sei ;
            videoPacketInfo->sei = NULL ;
        }
        videoPacketInfo->nSeiSize = 0 ;
    }
}

#ifdef __APPLE__
void TYBaseVideoDecoder::draw_frames_for_image(const std::shared_ptr<TYVideoPacketInfo> &videoPacketInfo,CVImageBufferRef imageBuffer){
    if (videoPacketInfo->sei && videoPacketInfo->nSeiSize > 0 && m_bIVAEnable) {
        nObjsNumbers = 0 ;
        if (activeObjs) {
            delete[] activeObjs ;
            activeObjs = NULL ;
        }
        get_point_pos_(videoPacketInfo->sei,videoPacketInfo->nSeiSize,&activeObjs,nObjsNumbers);
    }
    
    if (nObjsNumbers > 0 && activeObjs) {
        for (int i = 0; i < nObjsNumbers; i ++) {
            draw_one_frame_ios(imageBuffer, activeObjs[i]);
        }
    }
}

void TYBaseVideoDecoder::draw_one_frame_ios(CVImageBufferRef imageBuffer, objectRect objs){
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    OSType imageType = CVPixelBufferGetPixelFormatType(imageBuffer);
    if (kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange == imageType || kCVPixelFormatType_420YpCbCr8BiPlanarFullRange == imageType) {     // nv12
        int width = (int)CVPixelBufferGetWidth(imageBuffer);
        int height = (int)CVPixelBufferGetHeight(imageBuffer);
        int ystride = (int)CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 0);
        int uvstride = (int)CVPixelBufferGetBytesPerRowOfPlane(imageBuffer, 1);
        unsigned char *y = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
        unsigned char *uv = (unsigned char *) CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 1);
        
        objs.topx = objs.topx / 2 * 2 ;
        objs.topx = objs.topx > 0 ? objs.topx : 0 ;
        objs.topy = objs.topy / 2 * 2 ;
        objs.topy = objs.topy > 0 ? objs.topy : 0 ;
        
        objs.botx = objs.botx / 2 * 2 ;
        objs.botx = objs.botx < (width - 1 - FRAME_HEIGHT) ? objs.botx : (width - 1 - FRAME_HEIGHT);
        objs.boty = objs.boty / 2 * 2 ;
        objs.boty = objs.boty < (height - 1 - FRAME_HEIGHT) ? objs.boty : (height - 1 - FRAME_HEIGHT);
        // draw y top line
        for (int i = 0; i < FRAME_HEIGHT; i ++) {
            memset(y + (objs.topy + i) * ystride + objs.topx, 0xff, objs.botx - objs.topx);
            memset(y + (objs.boty + i) * ystride + objs.topx, 0xff, objs.botx - objs.topx);
        }
        int half_of_height = FRAME_HEIGHT / 2;
        for (int i = 0; i < half_of_height ; i ++) {
            memset(uv + (objs.topy /2 + i) * uvstride + objs.topx , 0x80, (objs.botx - objs.topx)/2 * 2);   // uv * 2
            memset(uv + (objs.boty /2 + i) * uvstride + objs.topx , 0x80, (objs.botx - objs.topx)/2 * 2);   // uv * 2
        }
        // draw left line
        for (int i = objs.topy; i < objs.boty ; i++) {
            for (int j = 0; j < FRAME_HEIGHT; j ++) {
                *(y + i * ystride + objs.topx + j) = 0xff ;
                *(y + i * ystride + objs.botx + j) = 0xff ;
            }
        }
        int half_of_boty = objs.boty / 2;
        for (int i = objs.topy /2; i < half_of_boty; i++) {
            for (int j = 0; j < FRAME_HEIGHT; j ++) {
                *(uv + i * uvstride + objs.topx + j) = 0x80;
                *(uv + i * uvstride + objs.botx + j) = 0x80;
            }
        }
    }
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
}
#endif

void TYBaseVideoDecoder::EnableIVA(bool enable){
    m_bIVAEnable = enable ;
}

void TYBaseVideoDecoder::onLastVideoFrameArrived(){
    m_bLastFrameArrive = true ;
}
