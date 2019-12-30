//
//  TYFrameInfo.cpp
//  TYCameraSdk
//
//  Created by Jim on 2018/6/26.
//  Copyright © 2018年 Tuya. All rights reserved.
//

#include "TYPublic.h"
#include "TYFrameInfo.h"

#ifdef __APPLE__
    #include <CoreFoundation/CoreFoundation.h>
#endif

//void TYFreeAudioPacket(TYAudioPacketInfo *audioPacketInfo){
//    if (audioPacketInfo){
//        TY_DELETE_ARRAY(audioPacketInfo->data);
//        TY_DELETE(audioPacketInfo);
//    }
//}
//
//void TYFreeVideoPacket(TYVideoPacketInfo *videoPacketInfo){
//    if (videoPacketInfo){
//        TY_DELETE_ARRAY(videoPacketInfo->data);
//        TY_DELETE(videoPacketInfo);
//    }
//}
//
//void TYFreeVideoFrame(TYVideoFrameInfo *videoFrameInfo){
//    if (videoFrameInfo){
//#ifdef __APPLE__
//        CFRelease(CFTypeRef(videoFrameInfo->data));
//#elif defined(__ANDROID__)
//        TY_DELETE_ARRAY(videoFrameInfo->y);
//        TY_DELETE_ARRAY(videoFrameInfo->u);
//        TY_DELETE_ARRAY(videoFrameInfo->v);
//#endif
//        
//        TY_DELETE(videoFrameInfo);
//    }
//}
//
//void TYFreeAudioFrame(TYAudioFrameInfo *audioFrameInfo){
//    if (audioFrameInfo){
//        TY_DELETE_ARRAY(audioFrameInfo->data);
//        TY_DELETE(audioFrameInfo);
//    }
//}
//
//void TYAudioPacketRetain(TYAudioPacketInfo *audioPacketInfo){
//    if (audioPacketInfo){
//        pthread_mutex_lock(&audioPacketInfo->mutexRetainCount);
//        audioPacketInfo->nRetainCount ++;
//        pthread_mutex_unlock(&audioPacketInfo->mutexRetainCount);
//    }
//}
//
//void TYVideoPacketRetain(TYVideoPacketInfo *videoPacketInfo){
//    if (videoPacketInfo){
//        pthread_mutex_lock(&videoPacketInfo->mutexRetainCount);
//        videoPacketInfo->nRetainCount ++;
//        pthread_mutex_unlock(&videoPacketInfo->mutexRetainCount);
//    }
//}
//
//void TYAudioPacketRelease(TYAudioPacketInfo *audioPacketInfo){
//    if (audioPacketInfo){
//        pthread_mutex_lock(&audioPacketInfo->mutexRetainCount);
//        audioPacketInfo->nRetainCount --;
//        pthread_mutex_unlock(&audioPacketInfo->mutexRetainCount);
//    }
//    
//    if (audioPacketInfo && 0 == audioPacketInfo->nRetainCount){
//        TYFreeAudioPacket(audioPacketInfo);
//    }
//}
//
//void TYVideoPacketRelease(TYVideoPacketInfo *videoPacketInfo){
//    if (videoPacketInfo){
//        pthread_mutex_lock(&videoPacketInfo->mutexRetainCount);
//        videoPacketInfo->nRetainCount --;
//        pthread_mutex_unlock(&videoPacketInfo->mutexRetainCount);
//    }
//    
//    if (videoPacketInfo && 0 == videoPacketInfo->nRetainCount){
//        TYFreeVideoPacket(videoPacketInfo);
//    }
//}
//
//void TYVideoFrameRetain(TYVideoFrameInfo *videoFrameInfo){
//    if (videoFrameInfo){
//        pthread_mutex_lock(&videoFrameInfo->mutexRetainCount);
//        videoFrameInfo->nRetainCount ++;
//        pthread_mutex_unlock(&videoFrameInfo->mutexRetainCount);
//    }
//}
//
//void TYVideoFrameRelease(TYVideoFrameInfo *videoFrameInfo){
//    if (videoFrameInfo){
//        pthread_mutex_lock(&videoFrameInfo->mutexRetainCount);
//        videoFrameInfo->nRetainCount --;
//        pthread_mutex_unlock(&videoFrameInfo->mutexRetainCount);
//    }
//    
//    if (videoFrameInfo && 0 == videoFrameInfo->nRetainCount){
//        TYFreeVideoFrame(videoFrameInfo);
//    }
//}
//
//void TYAudioFrameRetain(TYAudioFrameInfo *audioFrameInfo){
//    if (audioFrameInfo){
//        pthread_mutex_lock(&audioFrameInfo->mutexRetainCount);
//        audioFrameInfo->nRetainCount ++;
//        pthread_mutex_unlock(&audioFrameInfo->mutexRetainCount);
//    }
//}
//
//void TYAudioFrameRelease(TYAudioFrameInfo *audioFrameInfo){
//    if (audioFrameInfo){
//        pthread_mutex_lock(&audioFrameInfo->mutexRetainCount);
//        audioFrameInfo->nRetainCount --;
//        pthread_mutex_unlock(&audioFrameInfo->mutexRetainCount);
//    }
//    
//    if (audioFrameInfo && 0 == audioFrameInfo->nRetainCount){
//        TYFreeAudioFrame(audioFrameInfo);
//    }
//}
//
//void TYFreeVideoDataContainer(TYVideoFrameContainer *videoFrameContainer){
//    if (videoFrameContainer){
//        TYVideoFrameRelease(videoFrameContainer->videoFrameInfo);
//        TYVideoPacketRelease(videoFrameContainer->videoPacketInfo);
//        TY_DELETE(videoFrameContainer);
//    }
//}
//
//void TYFreeAudioDataContainer(TYAudioFrameContainer *audioFrameContainer){
//    if (audioFrameContainer){
//        TY_DELETE_ARRAY(audioFrameContainer->audioFrameInfo->data);
//        TY_DELETE(audioFrameContainer->audioFrameInfo);
//        
//        TYAudioPacketRelease(audioFrameContainer->audioPacketInfo);
//        TY_DELETE(audioFrameContainer);
//    }
//}
