//
//  MNPublic.h
//  TYCameraSdk
//
//  Created by Jim on 2018/3/8.
//  Copyright © 2018年 TuyaSmart. All rights reserved.
//

#ifndef __TY_PUBLIC_H__
#define __TY_PUBLIC_H__

#define TY_PROCESS_ERROR(Condition)             \
do                                              \
{                                               \
    if (!(Condition))                           \
        goto Exit0;                             \
} while (false)


#define TY_PROCESS_SUCCESS(Condition)           \
do                                              \
{                                               \
    if (Condition)                              \
        goto Exit1;                             \
} while (false)

#define TY_LOG_PROCESS_ERROR(Condition, Msg)    \
do                                              \
{                                               \
    if (!(Condition)){                          \
        fprintf(stderr, "%s\n", Msg);           \
        goto Exit0;                             \
    }                                           \
} while (false)

#define TY_DELETE_ARRAY(pArray)                 \
do                                              \
{                                               \
    if (pArray)                                 \
    {                                           \
        delete [](pArray);                      \
        (pArray) = NULL;                        \
    }                                           \
} while (false)


#define TY_DELETE(p)                            \
do                                              \
{                                               \
    if (p)                                      \
    {                                           \
        delete (p);                             \
        (p) = NULL;                             \
    }                                           \
} while (false)

#define TY_CLOSE_FILE_SAFELY(p)                 \
do{                                             \
    if (p){                                     \
        fclose((p));                            \
        (p) = NULL;                             \
    }                                           \
} while (false)                                 

#ifdef __APPLE__
    #ifdef DEBUG
        #define  LOGI(FORMAT, ...) fprintf(stderr,FORMAT, ##__VA_ARGS__)
        #define  LOGE printf
    #else
        #define LOGI(FORMAT, ...)
        #define LOGE
    #endif
#elif defined(__ANDROID__)
    #include <android/log.h>
    #define  LOG_TAG    "TYSDK"
    #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#endif  // __TY_PUBLIC_H__
