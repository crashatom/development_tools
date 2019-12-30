//
//  FullBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef FullBox_hpp
#define FullBox_hpp

#include "BasicBox.h"

class FullBox: public BasicBox{
public:
    FullBox(){};
    virtual ~FullBox(){};
    
    virtual int ParseData(uint8_t *&data, size_t &data_size){
        int boxSize = 0;
        
        boxHeader.size = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(boxHeader.size);
        
        boxHeader.type = Bytes2String(data, 4, data_size, true);
        boxSize += boxHeader.type.length();
        
        version = Bytes2Int(data, data_size, 1, true);
        boxSize += sizeof(version);
        
        memcpy(flags, data, sizeof(flags));
        data += sizeof(flags);
        boxSize += sizeof(flags);
        data_size -= sizeof(flags);
        
        return boxSize;
    };
    virtual int Describe() { printf("basic box.\n"); return ErrCode_NoErr; };
public:
    BoxHeader boxHeader;
    
    uint8_t version;
    uint8_t flags[3];
    
};


#endif /* FullBox_hpp */
