/*
jiangping
*/
#import <Foundation/Foundation.h>
#import "IJKStreamMeta.h"

@interface IJKMeta : NSObject

@property(nonatomic)NSMutableDictionary *mMediaMeta;
@property(nonatomic)IJKStreamMeta *mVideoStream;//后续添加
@property(nonatomic)IJKStreamMeta *mAudioStream;
@property(nonatomic)IJKStreamMeta *mSubtitleStream;

@property(nonatomic) NSString *mFormat;
@property(nonatomic) int64_t mDurationUS;
@property(nonatomic) int64_t mStartUS;
@property(nonatomic) int64_t mBitrate;

@property(nonatomic) NSMutableArray *mStreams;

- (NSString *)getStringfromDict:(NSString *)key;
- (int64_t)getInt64fromDict:(NSString *)key;
- (int)getIntfromDict:(NSString *)key;
+ (IJKMeta *)parse:(NSMutableDictionary *)mediaMeta;

@end