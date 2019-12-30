//
//  stszBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stszBox.hpp"

stszBox::stszBox(){
    
}

stszBox::~stszBox(){
    
}

BasicBox *stszBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stszBox;
    
    return box;
}

int stszBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    sample_size = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(sample_size);
    
    number_of_entries = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(number_of_entries);
    
    while (boxSize < boxHeader.size){
        uint32_t sample_size = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(sample_size);
        
        sample_size_table.push_back(sample_size);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stszBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    printf("version\t\t\t\t: %d\n", version);
    printf("flags\t\t\t\t: 0x%x 0x%x 0x%x\n", flags[0], flags[1], flags[2]);
    printf("sample size: %d\n", sample_size);
    printf("number_of_entries: %d\n", number_of_entries);
    for (int i = 0; i < sample_size_table.size(); i++){
        printf("sample %d size\t\t\t\t: %d\n", i + 1, sample_size_table[i]);
    }
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}

