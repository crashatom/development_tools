//
//  stcoBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stcoBox.hpp"

stcoBox::stcoBox(){
    
}

stcoBox::~stcoBox(){
    
}

BasicBox *stcoBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stcoBox;
    
    return box;
}

int stcoBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    num_of_entries = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(num_of_entries);
    
    while (boxSize < boxHeader.size){
        uint32_t chunk_offset = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(chunk_offset);
        
        chunkOffsets.push_back(chunk_offset);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stcoBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    printf("version\t\t\t\t: %d\n", version);
    printf("flags\t\t\t\t: 0x%x 0x%x 0x%x\n", flags[0], flags[1], flags[2]);
    printf("number_of_entries: %d\n", num_of_entries);
    for (int i = 0; i < chunkOffsets.size(); i++){
        printf("file chunk%d offset\t\t\t\t: %d\n", i + 1, chunkOffsets[i]);
    }
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
