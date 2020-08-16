/*
 * IJKFFMoviePlayerController.h
 *
 * Copyright (c) 2013 Bilibili
 * Copyright (c) 2013 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#import "IJKMediaPlayback.h"
#import "IJKFFMonitor.h"
#import "IJKFFOptions.h"
#import "IJKSDLGLViewProtocol.h"
#import "IJKMediaMeta.h"
#import "IJKStreamMeta.h"

// media meta
#define k_IJKM_KEY_FORMAT         @"format"
#define k_IJKM_KEY_DURATION_US    @"duration_us"
#define k_IJKM_KEY_START_US       @"start_us"
#define k_IJKM_KEY_BITRATE        @"bitrate"

// stream meta
#define k_IJKM_KEY_TYPE           @"type"
#define k_IJKM_VAL_TYPE__VIDEO    @"video"
#define k_IJKM_VAL_TYPE__AUDIO    @"audio"
#define k_IJKM_VAL_TYPE__UNKNOWN  @"unknown"

#define k_IJKM_KEY_CODEC_NAME      @"codec_name"
#define k_IJKM_KEY_CODEC_PROFILE   @"codec_profile"
#define k_IJKM_KEY_CODEC_LONG_NAME @"codec_long_name"

// stream: video
#define k_IJKM_KEY_WIDTH          @"width"
#define k_IJKM_KEY_HEIGHT         @"height"
#define k_IJKM_KEY_FPS_NUM        @"fps_num"
#define k_IJKM_KEY_FPS_DEN        @"fps_den"
#define k_IJKM_KEY_TBR_NUM        @"tbr_num"
#define k_IJKM_KEY_TBR_DEN        @"tbr_den"
#define k_IJKM_KEY_SAR_NUM        @"sar_num"
#define k_IJKM_KEY_SAR_DEN        @"sar_den"
// stream: audio
#define k_IJKM_KEY_SAMPLE_RATE    @"sample_rate"
#define k_IJKM_KEY_CHANNEL_LAYOUT @"channel_layout"

#define kk_IJKM_KEY_STREAMS       @"streams"

typedef enum IJKLogLevel {
    k_IJK_LOG_UNKNOWN = 0,
    k_IJK_LOG_DEFAULT = 1,

    k_IJK_LOG_VERBOSE = 2,
    k_IJK_LOG_DEBUG   = 3,
    k_IJK_LOG_INFO    = 4,
    k_IJK_LOG_WARN    = 5,
    k_IJK_LOG_ERROR   = 6,
    k_IJK_LOG_FATAL   = 7,
    k_IJK_LOG_SILENT  = 8,
} IJKLogLevel;

@interface IJKFFMoviePlayerController : NSObject <IJKMediaPlayback>
 - (id)initWithOptions:(IJKFFOptions *)options;
- (id)initWithContentURL:(NSURL *)aUrl
             withOptions:(IJKFFOptions *)options;

- (id)initWithContentURLString:(NSString *)aUrlString
                   withOptions:(IJKFFOptions *)options;

- (id)initWithMoreContent:(NSURL *)aUrl
             withOptions:(IJKFFOptions *)options
              withGLView:(UIView<IJKSDLGLViewProtocol> *)glView;

- (id)initWithMoreContentString:(NSString *)aUrlString
                 withOptions:(IJKFFOptions *)options
                  withGLView:(UIView<IJKSDLGLViewProtocol> *)glView;
- (void)setPlayerUrl : (NSString *)url; - (NSString *)getPlayerUrl;
- (void)prepareToPlay;
- (void)play;
- (void)pause;
- (void)stop;
- (BOOL)isPlaying;
- (int64_t)trafficStatistic;
- (float)dropFrameRate;
//*************
//IJKFFMoviePlayerController.m 已声明，但h文件未声�?
- (void)setPlaybackRate:(float)playbackRate;
- (float)getPlaybackRate;
- (void)setPlaybackVolume:(float)volume;
- (float)getPlaybackVolume;
- (int64_t)getFileSize;

//添加新接�?
- (int)getSelectedTrack:(int)type;
- (void)selectTrack:(int)track;
- (void)deselectTrack:(int)track;- (void)selectVideoTrack:(int)track;- (void)selectAudioTrack:(NSString *)lang;- (void)selectSubtitleTrack:(NSString *)lang;
- (int)getVideoWidth;
- (int)getVideoHeight;
- (int)getVideoSarNum;
- (int)getVideoSarDen;
- (void)setLooping:(BOOL)looping;
- (BOOL)isLooping;

- (int)getVideoDecoder;
- (float)getVideoOutputFramesPerSecond;
- (float)getVideoDecodeFramesPerSecond;
- (int64_t)getVideoCachedDuration;
- (int64_t)getAudioCachedDuration;
- (int64_t)getVideoCachedBytes;
- (int64_t)getAudioCachedBytes;
- (int64_t)getVideoCachedPackets;
- (int64_t)getAudioCachedPackets;
- (int64_t)getAsyncStatisticBufBackwards;
- (int64_t)getAsyncStatisticBufForwards;
- (int64_t)getAsyncStatisticBufCapacity;
- (int64_t)getTrafficStatisticByteCount;
- (int64_t)getCacheStatisticPhysicalPos;
- (int64_t)getCacheStatisticFileForwards;
- (int64_t)getCacheStatisticFilePos;
- (int64_t)getBitRate;
- (int64_t)getTcpSpeed;
- (int64_t)getSeekLoadDuration;
- (float)getDropFrameRate;
- (int)getVideoTrackSum; - (int)getAudioTrackSum; - (int)getSubtitleTrackSum; - (NSMutableArray *)getVideoLists; - (NSMutableArray *)getAudioLists; - (NSMutableArray *)getSubtitleLists;
- (void)setCacheShare:(int)share;
- (NSMutableDictionary *)getParsedTypeInfo:(int) meta_type;
- (void)setInitBufferTime:(int64_t)msTime;
- (void)setPlaybackBufferTime:(int64_t)msTime;
- (void)setMaxBufferTime:(int64_t)msTime;
- (void)setMaxAllBufferSize:(int64_t)msTime;
- (void)setMaxBufferSize:(int64_t)msTime;

- (int)startRecord:(NSString*) filename;
- (int)stopRecord;
- (int)getRecordStatus;
+ (void)setSaveLogPathName:(NSString*) strSavelogPathName;
- (NSString *)getProbeKPI;

- (void)setMute;
//- (IJKMeta *)getMediaMeta;- (NSMutableArray *)getTrackInfo;
- (NSMutableArray *)getVideoTrackInfo;- (NSMutableArray *)getAudioTrackInfo;- (NSMutableArray *)getSubtitleTrackInfo;- (IJKStreamMeta *)getCurVideoInfo;- (IJKStreamMeta *)getCurAudioInfo;- (IJKStreamMeta *)getCurSubtitleInfo;

//*************
- (void)setPauseInBackground:(BOOL)pause;
- (BOOL)isVideoToolboxOpen;

- (void)setHudValue:(NSString *)value forKey:(NSString *)key;

+ (void)setLogReport:(BOOL)preferLogReport;
+ (void)setLogLevel:(IJKLogLevel)logLevel;
+ (BOOL)checkIfFFmpegVersionMatch:(BOOL)showAlert;
+ (BOOL)checkIfPlayerVersionMatch:(BOOL)showAlert
                            version:(NSString *)version;

@property(nonatomic, readonly) CGFloat fpsInMeta;
@property(nonatomic, readonly) CGFloat fpsAtOutput;
@property(nonatomic) BOOL shouldShowHudView;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(IJKFFOptionCategory)category;

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(IJKFFOptionCategory)category;



- (void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
- (void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
- (void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
- (void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

- (void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
- (void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
- (void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
- (void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

@property (nonatomic, retain) id<IJKMediaUrlOpenDelegate> segmentOpenDelegate;
@property (nonatomic, retain) id<IJKMediaUrlOpenDelegate> tcpOpenDelegate;
@property (nonatomic, retain) id<IJKMediaUrlOpenDelegate> httpOpenDelegate;
@property (nonatomic, retain) id<IJKMediaUrlOpenDelegate> liveOpenDelegate;

@property (nonatomic, retain) id<IJKMediaNativeInvokeDelegate> nativeInvokeDelegate;
- (void)didShutdown;

#pragma mark KVO properties
@property (nonatomic, readonly) IJKFFMonitor *monitor;

@end

#define IJK_FF_IO_TYPE_READ (1)
void IJKFFIOStatDebugCallback(const char *url, int type, int bytes);
void IJKFFIOStatRegister(void (*cb)(const char *url, int type, int bytes));

void IJKFFIOStatCompleteDebugCallback(const char *url,
                                      int64_t read_bytes, int64_t total_size,
                                      int64_t elpased_time, int64_t total_duration);
void IJKFFIOStatCompleteRegister(void (*cb)(const char *url,
                                            int64_t read_bytes, int64_t total_size,
                                            int64_t elpased_time, int64_t total_duration));
