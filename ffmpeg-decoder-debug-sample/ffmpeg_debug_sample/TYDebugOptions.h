#ifndef __TYDebugOptions_H__
#define __TYDebugOptions_H__

//#define DEBUG_SHOW_MP4_INFO                     1
//#define DEBUG_AV_SYNC_TS_INFO                   1
//#define DEBUG_SHOW_AUTH_INFO                    1               // 打印p2p鉴权信息
//#define DEBUG_SHOW_RECV_COMMAND                 1               // 打印收到的信令
//#define DEBUG_SHOW_SEND_COMMAND                 1               // 打印收到的信令
//#define DEBUG_WRITE_YUV_DATA_TO_FILE            1
//#define DEBUG_SHOW_VIDEO_FRAME_DECODE_TIME      1               // 打印视频解码耗时
//#define DEBUG_SHOW_RTP_H264_TIMESTAMP           1
//#define DEBUG_TASK_WRITE_H264_DATA              1
//#define DEBUG_SHOW_PTHREAD_LIFETIME             1
//#define DEBUG_SHOW_AUDIO_DELAY                  1
//#define DEBUG_SHOW_VIDEO_DELAY                  1
//#define DEBUG_SHOW_AUDIO_CACHED_DATA_NUM        1               // 打印视频缓存数量
//#define DEBUG_SHOW_VIDEO_CACHED_DATA_NUM        1               // 打印视频缓存数量
//#define DEBUG_SHOW_EXPORT_API_INVOKE_INFO       1               // 打印视频缓存数量
//#define DEBUG_LOG_TO_NATIVE_FILE                1
//#define DEBUG_SHOW_AUDIO_DATA_SEND_INFO         1               // 打印音频数据发送信息
//#define DEBUG_HEVC_RTP_DEBUG                    1               // 打印HEVC rtp包debug信息
//#define DEBUG_VERBOSE_WARNING                   1               // 冗余调试日志
//#define DEBUG_MEDIACODEC_INFO                   1               // MediaCodec 调试日志
//#define DEBUG_IVA_SEI_INFO                      1               // SEI 日志
//#define DEBUG_FILE_HANDLE_CHECK                 1               // 文件句柄泄漏调试日志

// 调试选项
#define DEBUG_SHOW_EXPORT_API_INVOKE_INFO_TAG   "invoke"        // 打印视频缓存数量
#define DEBUG_SHOW_VIDEO_CACHED_DATA_NUM_TAG    "vcache"        // 打印视频缓存数量
#define DEBUG_SHOW_AUDIO_CACHED_DATA_NUM_TAG    "acache"        // 打印视频缓存数量
#define DEBUG_SHOW_VIDEO_DELAY_TAG              "vdelay"
#define DEBUG_SHOW_AUDIO_DELAY_TAG              "vdelay"
#define DEBUG_SHOW_PTHREAD_LIFETIME_TAG         "thread"
#define DEBUG_SHOW_RTP_H264_TIMESTAMP_TAG       "rtp.timestamp"
#define DEBUG_SHOW_VIDEO_FRAME_DECODE_TIME_TAG  "vdecode.time"  // 打印视频解码耗时
#define DEBUG_SHOW_SEND_COMMAND_TAG             "cmd"
#define DEBUG_SHOW_RECV_COMMAND_TAG             "cmd"
#define DEBUG_SHOW_AUTH_INFO_TAG                "auth.info"
#define DEBUG_AV_SYNC_TS_INFO_TAG               "avsync"
#define DEBUG_SHOW_MP4_INFO_TAG                 "mp4"
#define DEBUG_FILE_HANDLE_CHECK_TAG             "file"
#define DEBUG_HEVC_RTP_DEBUG_TAG                "hevc"
#define DEBUG_SHOW_AUDIO_DATA_SEND_INFO_TAG     "audio.talk"
#define DEBUG_VERBOSE_WARNING_TAG               "warning"
#define DEBUG_MEDIACODEC_INFO_TAG               "mediacodec"
#define DEBUG_IVA_SEI_INFO_TAG                  "IVA"

#endif // __TYDebugOptions_H__
