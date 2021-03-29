#include <fcntl.h>

#include "TYiOSVideoHWDecoder.h"
#include "TYFFMpegVideoDecoder.h"

#import "ViewController.h"

const uint8_t nalu_leading_code[2][5] = {
    {4, 0x00, 0x00, 0x00, 0x01},    // 4 byte leading code.
    {3, 0x00, 0x00, 0x01, 0x00},    // 3 byte leading code.
};

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [NSThread detachNewThreadWithBlock:^{
        [self beginTest];
    }];
}

uint8_t *GetH264NaluFromHead(uint8_t *pInData, long nInSize, int &nNaluSizeContainsLeadingCode, int &nNaluType, int &nLeadingCodeSize){
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

uint8_t *GetOneFrame(uint8_t *srcData, size_t *srcDataSize, uint8_t *frameData, size_t *frameSize){
    uint8_t *data = srcData;
    uint8_t *frame = frameData;
    *frameSize = 0;
    int size = (int)*srcDataSize;
    
    int flag = 0;
    int nNaluType = 0;
    int nLeadingCodeSize = 0;
    int nNaluSizeContainsLeadingCode = 0;
    
    while ((data = GetH264NaluFromHead(data, size, nNaluSizeContainsLeadingCode, nNaluType, nLeadingCodeSize)) != nullptr){
        
        if ((nNaluType&0x1F) == 1 || (nNaluType&0x1F) == 7){
            if (flag <= 0){
                flag ++;
            }else{
                *srcDataSize -= *frameSize;
                return srcData + *frameSize;
            }
        }
        
        memcpy(frame + *frameSize, data, nNaluSizeContainsLeadingCode);
        *frameSize += nNaluSizeContainsLeadingCode;
        data += nNaluSizeContainsLeadingCode;
    }
    
    return NULL;
}

-(void)beginTest{
    
//    uint8_t *_data = nullptr;
//
//    _data = (uint8_t *)realloc(_data, 10);
//    uint8_t *src = (uint8_t *)realloc(_data, 0);
//    free(_data);
    
    
    // 设置数据文件名
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"out" ofType:@".h264"];
    
    FILE *file = fopen([filePath UTF8String], "r");
    
    fseek(file, 0, SEEK_END);   // 将文件指针移动文件结尾
    size_t totalSize = ftell(file);  // 求出当前文件指针距离文件开始的字节数
    fseek(file, 0, SEEK_SET);
    uint8_t *src_data = new uint8_t[totalSize+1];
    
    fread(src_data, totalSize, 1, file);
    
    uint8_t *data = src_data;
    uint8_t *OneFrame = new uint8_t[1024*1024];
    size_t frameSize = 0;
    
//    TYiOSVideoHardwareDecoder *decoder = new TYiOSVideoHardwareDecoder("video/avc");
    TYFFMpegVideoDecoder *decoder = new TYFFMpegVideoDecoder("video/avc");
    
    std::shared_ptr<TYVideoPacketInfo> videoPacketInfo = std::make_shared<TYVideoPacketInfo>();
    videoPacketInfo->codec_id = TY_AV_CODEC_VIDEO_H264;
    while ((data = GetOneFrame(data, &totalSize, OneFrame, &frameSize)) != nullptr){
        videoPacketInfo->data = OneFrame;
        videoPacketInfo->nDataSize = frameSize;
        
        decoder->DecodeNetStreamWithData(videoPacketInfo, NULL);
    }
}

@end
