//
//  Utils.c
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "Utils.h"
#include "ErrCode.h"

int Bytes2Int(uint8_t *&data, size_t &data_size, int byteCount, bool shift){
    int result = 0;
    
    // byteCount字节大端转小端
    int offset_bytes = 0;
    for (int i = byteCount - 1; i >= 0; i--)
    {
        result += data[i] << (offset_bytes * 8);
        offset_bytes++;
    }
    
    if (shift){
        data += byteCount;
        data_size -= byteCount;
    }

    return result;
}

std::string Bytes2String(uint8_t *&data, int strLen, size_t &data_size, bool doShift){
    std::string result((char *)data, strLen);
    
    if (doShift){
        data += strLen;
        data_size -= strLen;
    }
    
    return result;
}
