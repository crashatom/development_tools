//
//  Utils.h
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <string>
#include <stdio.h>
#include <stdint.h>


int Bytes2Int(uint8_t *&data, size_t &data_size, int byteCount, bool shift);
std::string Bytes2String(uint8_t *&data, int strLen, size_t &data_size, bool doShift);

#endif /* Utils_h */
