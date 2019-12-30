//
//  BasicBox.h
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef BasicBox_h
#define BasicBox_h

#include <string>
#include <vector>
#include <stdio.h>

#include "Utils.h"
#include "ErrCode.h"

struct BoxHeader {
    int size;
    std::string type;
};

class BasicBox{
public:
    BasicBox(){};
    virtual ~BasicBox(){};
    
    virtual int ParseData(uint8_t *&data, size_t &data_size){ return ErrCode_NoErr; };
    virtual int Describe() { printf("basic box.\n"); return ErrCode_NoErr; };
public:
    BoxHeader boxHeader;
    
    std::vector<BasicBox *> subBoxes;
};


#endif /* BasicBox_h */
