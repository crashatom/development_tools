/*
jiangping
*/
#import <Foundation/Foundation.h>


@interface IJKStreamMeta : NSObject

@property(nonatomic)NSDictionary *mMeta;

@property(nonatomic) int mIndex;
@property(nonatomic) NSString *mType;
@property(nonatomic) NSString *mLanguage;


@property(nonatomic) int64_t mDurationUS;
@property(nonatomic) int64_t mStartUS;
@property(nonatomic) int64_t mBitrate;

// common
@property(nonatomic) NSString *mCodecName;
@property(nonatomic) NSString *mCodecProfile;
@property(nonatomic) NSString *mCodecLongName;

// video
@property(nonatomic) int mWidth;
@property(nonatomic) int mHeight;
@property(nonatomic) int mFpsNum;
@property(nonatomic) int mFpsDen;
@property(nonatomic) int mTbrNum;
@property(nonatomic) int mTbrDen;
@property(nonatomic) int mSarNum;
@property(nonatomic) int mSarDen;

// audio
@property(nonatomic) int mSampleRate;
@property(nonatomic) int64_t mChannelLayout;

- (void)initWithIndex:(int)index;
- (NSString *)getStringfromDict:(NSString *)key;
- (int64_t)getInt64fromDict:(NSString *)key;
- (int)getIntfromDict:(NSString *)key;
+ (IJKStreamMeta *)parse:(NSMutableDictionary *)streamMeta;

@end