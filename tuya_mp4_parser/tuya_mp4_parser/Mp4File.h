//
//  Mp4File.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef Mp4File_hpp
#define Mp4File_hpp

#include <vector>
#include <string>
#include <stdio.h>

#include "BasicBox.h"
#include "BoxFactory.hpp"

class Mp4File{
public:
    Mp4File();
    ~Mp4File();
    
public:
    int ParseData(uint8_t *data, size_t size);
    int LoadFile(const char *path);
    int PrintOverviewOfVideoFrames(int maxNumberOfBytesToPrintPerFrame);
    
    BasicBox *GetBoxFromData(uint8_t *data, size_t &dataSize);
    BasicBox *CreateBox(std::string boxName);
    
public:
    std::vector<BasicBox *> boxes;
    uint8_t *fileData;
};

#endif /* Mp4File_hpp */
