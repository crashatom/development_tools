//
//  mdatBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef mdatBox_hpp
#define mdatBox_hpp

#include "BasicBox.h"

// unsigned int len;
// unsigned char name[4];
// sample0{
//     unsigned int sampleSize;//不包含（sampleSize本身长度）的sample长度
//     unsigned int sample[sampleSize];//去掉0001分隔符的H264裸数据
// }
// sample1
// ...
// samplen


class mdatBox: public BasicBox{
    struct Sample{
        size_t   size;
        uint8_t *data;
    };
    
public:
    mdatBox();
    ~mdatBox();
    
    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    std::vector<Sample *> samples;
};

#endif /* mdatBox_hpp */
