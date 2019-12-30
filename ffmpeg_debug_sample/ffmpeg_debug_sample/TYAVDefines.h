//
//  TYAVDefines.h
//  TYCameraSdk
//
//  Created by Jim on 2018/4/8.
//  Copyright © 2018年 Tuya. All rights reserved.
//

#ifndef __TYAVDefines_H__
#define __TYAVDefines_H__

#define UNINITIALIZED -1

#pragma pack(4)

typedef enum{
    MEDIA_TYPE_VIDEO,
    MEDIA_TYPE_AUDIO,
}MediaType;

typedef struct tagTuyaAVParams{
    int nVideoCodecId;
    int nFrameRate;
    int nWidth;
    int nHeight;
    
    int nAudioCodecId;
    int nSampRate;
    int nChannels;
    int nBitWidth;
}TuyaAVParams;

typedef enum{
    TUYA_CODEC_VIDEO_MPEG4      = 0,
    TUYA_CODEC_VIDEO_H263,
    TUYA_CODEC_VIDEO_H264,
    TUYA_CODEC_VIDEO_MJPEG,
    TUYA_CODEC_VIDEO_H265,
    TUYA_CODEC_VIDEO_MAX        = 99,
    
    TUYA_CODEC_AUDIO_ADPCM,
    TUYA_CODEC_AUDIO_PCM,
    TUYA_CODEC_AUDIO_AAC_RAW,
    TUYA_CODEC_AUDIO_AAC_ADTS,
    TUYA_CODEC_AUDIO_AAC_LATM,
    TUYA_CODEC_AUDIO_G711U,
    TUYA_CODEC_AUDIO_G711A,
    TUYA_CODEC_AUDIO_G726,
    TUYA_CODEC_AUDIO_SPEEX,
    TUYA_CODEC_AUDIO_MP3,
    TUYA_CODEC_AUDIO_MAX        = 199,
    TUYA_CODEC_INVALID
}TUYA_CLOUD_AV_CODEC_ID;

typedef enum{
    TY_AV_CODEC_UNKNOWN         = 0x00,
    
    TY_AV_CODEC_VIDEO_MPEG4     = 0x10,
    TY_AV_CODEC_VIDEO_H263      = 0x11,
    TY_AV_CODEC_VIDEO_H264      = 0x12,
    TY_AV_CODEC_VIDEO_MJPEG     = 0x13,
    TY_AV_CODEC_VIDEO_H265      = 0x14,
    
    TY_AV_CODEC_AUDIO_ADPCM     = 0X80,
    TY_AV_CODEC_AUDIO_PCM       = 0x81,
    TY_AV_CODEC_AUDIO_AAC_RAW   = 0x82,
    TY_AV_CODEC_AUDIO_AAC_ADTS  = 0x83,
    TY_AV_CODEC_AUDIO_AAC_LATM  = 0x84,
    TY_AV_CODEC_AUDIO_G711U     = 0x85,
    TY_AV_CODEC_AUDIO_G711A     = 0x86,
    TY_AV_CODEC_AUDIO_G726      = 0x87,
    TY_AV_CODEC_AUDIO_SPEEX     = 0x88,
    TY_AV_CODEC_AUDIO_MP3       = 0x89,
}TY_AV_CODEC_ID;

// 转换表
#ifndef __TUYA_CODEC_ID_CHARMS__
#define __TUYA_CODEC_ID_CHARMS__
    static int g_sAvTransferCodecId[][2] = {
        {   TUYA_CODEC_VIDEO_MPEG4,    TY_AV_CODEC_VIDEO_MPEG4     },
        {   TUYA_CODEC_VIDEO_H263,     TY_AV_CODEC_VIDEO_H263      },
        {   TUYA_CODEC_VIDEO_H264,     TY_AV_CODEC_VIDEO_H264      },
        {   TUYA_CODEC_VIDEO_MJPEG,    TY_AV_CODEC_VIDEO_MJPEG     },
        {   TUYA_CODEC_VIDEO_H265,     TY_AV_CODEC_VIDEO_H265      },
        {   TUYA_CODEC_AUDIO_ADPCM,    TY_AV_CODEC_AUDIO_ADPCM     },
        {   TUYA_CODEC_AUDIO_PCM,      TY_AV_CODEC_AUDIO_PCM       },
        {   TUYA_CODEC_AUDIO_AAC_RAW,  TY_AV_CODEC_AUDIO_AAC_RAW   },
        {   TUYA_CODEC_AUDIO_AAC_ADTS, TY_AV_CODEC_AUDIO_AAC_ADTS  },
        {   TUYA_CODEC_AUDIO_AAC_LATM, TY_AV_CODEC_AUDIO_AAC_LATM  },
        {   TUYA_CODEC_AUDIO_G711U,    TY_AV_CODEC_AUDIO_G711U     },
        {   TUYA_CODEC_AUDIO_G711A,    TY_AV_CODEC_AUDIO_G711A     },
        {   TUYA_CODEC_AUDIO_G726,     TY_AV_CODEC_AUDIO_G726      },
        {   TUYA_CODEC_AUDIO_SPEEX,    TY_AV_CODEC_AUDIO_SPEEX     },
        {   TUYA_CODEC_AUDIO_MP3,      TY_AV_CODEC_AUDIO_MP3       },
    };
#endif

typedef enum{
    TY_AUDIO_SAMPLE_8K,
    TY_AUDIO_SAMPLE_11K,
    TY_AUDIO_SAMPLE_12K,
    TY_AUDIO_SAMPLE_16K,
    TY_AUDIO_SAMPLE_22K,
    TY_AUDIO_SAMPLE_24K,
    TY_AUDIO_SAMPLE_32K,
    TY_AUDIO_SAMPLE_44K,
    TY_AUDIO_SAMPLE_48K,
    TY_AUDIO_SAMPLE_96K,
}TRANSFER_AUDIO_SAMPLE_E;

typedef enum{
    TY_AUDIO_DATABITS_8,
    TY_AUDIO_DATABITS_16,
}TY_AUDIO_DATABITS;

typedef enum{
    TY_AUDIO_CHANNEL_MONO,
    TY_AUDIO_CHANNEL_STERO,
}TRANSFER_AUDIO_CHANNEL_E;

typedef enum{
    // 透传
    TY_C2C_CMD_QUERY_TEXT,                                      // 0, 客户端向设备端透传字符串. [拓展某些功能时，避免重编sdk]
    
    // 查询类
    TY_C2C_CMD_QUERY_FIXED_ABILITY,                             // 1, 查询设备能力集，子类型见 TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE
    TY_C2C_CMD_QUERY_AUDIO_PARAMS,                              // 2, 查询音频参数，音频信息类型见 TY_CMD_QUERY_AUDIO_PARAMS
    TY_C2C_CMD_QUERY_PLAYBACK_INFO,                             // 3, 查询SD卡回放信息，参数见
    TY_C2C_CMD_QUERY_VIDEO_STREAM_PARAMS,                       // 4, 查询视频模式信息 { [通道号,高清/标准/流畅,宽高,编码类型], [通道号,高清/标准/流畅,宽高,编码类型], [通道号,高清/标准/流畅,宽高,编码类型] }
    TY_C2C_CMD_QUERY_VIDEO_CLARITY,                             // 5, 查询清晰度
    
    // IO控制类
    TY_C2C_CMD_IO_CTRL_VIDEO,                                   // 6, 直播命令，子类型见 TY_CMD_IO_CTRL_VIDEO
    TY_C2C_CMD_IO_CTRL_PLAYBACK,                                // 7, 回放命令，子类型见 TY_CMD_IO_CTRL_VIDEO
    TY_C2C_CMD_IO_CTRL_AUDIO,                                   // 8, 音频命令，子类型见 TY_CMD_IO_CTRL_AUDIO
    TY_C2C_CMD_IO_CTRL_VIDEO_CLARITY,                           // 9, 设置清晰度
    
    TY_C2C_CMD_PROTOCOL_VERSION,                                // 10, 协议版本
    
    // for camera of GW
    TY_C2C_CMD_QUERY_FIXED_ABILITY_GW,                          // 11, 查询设备能力集，子类型见 TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE
    TY_C2C_CMD_QUERY_AUDIO_PARAMS_GW,                           // 12, 查询音频参数，音频信息类型见 TY_CMD_QUERY_AUDIO_PARAMS .for camera of GW
    TY_C2C_CMD_QUERY_PLAYBACK_INFO_GW,                          // 13, 查询SD卡回放信息，参数见
    TY_C2C_CMD_QUERY_VIDEO_STREAM_PARAMS_GW,                    // 14, 查询视频模式信息 { [通道号,高清/标准/流畅,宽高,编码类型], [通道号,高清/标准/流畅,宽高,编码类型], [通道号,高清/标准/流畅,宽高,编码类型] }
    TY_C2C_CMD_QUERY_VIDEO_CLARITY_GW,                          // 15, 查询清
    TY_C2C_CMD_IO_CTRL_VIDEO_GW,                                // 16, 直播命令，子类型见 TY_CMD_IO_CTRL_VIDEO
    TY_C2C_CMD_IO_CTRL_PLAYBACK_GW,                             // 17, 回放命令，子类型见 TY_CMD_IO_CTRL_VIDEO
    TY_C2C_CMD_IO_CTRL_AUDIO_GW,                                // 18, 音频命令，子类型见 TY_CMD_IO_CTRL_AUDIO
    TY_C2C_CMD_IO_CTRL_VIDEO_CLARITY_GW,                        // 19, 设置清晰度
    
}TY_MAIN_CMD_TYPE;

typedef enum{
    TY_C2C_CMD_IO_CTRL_COMMAND_INVALID,                         // 命令无效
    TY_C2C_CMD_IO_CTRL_COMMAND_RECV,                            // 命令已收到
    TY_C2C_CMD_IO_CTRL_COMMAND_FAILED,                          // 命令执行失败
    TY_C2C_CMD_IO_CTRL_COMMAND_SUCCESS,                         // 命令已完成
}TY_C2C_CMD_IO_CTRL_STATUS_CODE;

// 能力集类型
typedef enum{
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO        = 0x1,     // 设备端是否支持video
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER      = 0x2,     // 设备端是否支持扬声器
    TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC          = 0x4,     // 设备端是否支持麦克风
}TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE;

// 视频播放命令[回放、直播]
typedef enum{
    TY_CMD_IO_CTRL_VIDEO_PLAY,                                  // 0, 开始
    TY_CMD_IO_CTRL_VIDEO_PAUSE,                                 // 1, 暂停
    TY_CMD_IO_CTRL_VIDEO_RESUME,                                // 2, 恢复
    TY_CMD_IO_CTRL_VIDEO_STOP,                                  // 3, 停止
    TY_CMD_IO_CTRL_AUDIO_START,                                 // 4, IPC -> APP的音频
    TY_CMD_IO_CTRL_AUDIO_STOP,                                  // 5, IPC -> APP的音频
}TY_CMD_IO_CTRL_AV_OP;

// 音频播放命令
typedef enum{
    TY_CMD_IO_CTRL_AUDIO_SPEAKER_START,                         // APP -> IPC，开始对讲
    TY_CMD_IO_CTRL_AUDIO_SPEAKER_STOP,                          // APP -> IPC，结束对讲
}TY_CMD_IO_CTRL_SPEAKER_OP;

// 卡回放子命令
typedef enum {
    TY_C2C_CMD_QUERY_PLAYBACK_INFO_BY_MONTH,                    // 按月查询 有回放的天
    TY_C2C_CMD_QUERY_PLAYBACK_INFO_BY_DAY,                      // 按天查询指定某天的回放数据
}TY_C2C_SUB_CMD_QUERY_PLAYBACK_INFO;

#pragma pack()

#endif // __TYAVDefines_H__
