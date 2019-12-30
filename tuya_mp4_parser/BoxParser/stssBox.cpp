//
//  stssBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stssBox.hpp"

stssBox::stssBox(){
    
}

stssBox::~stssBox(){
    
}

BasicBox *stssBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stssBox;
    
    return box;
}

int stssBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    number_of_key_samples = Bytes2Int(data, data_size, sizeof(number_of_key_samples), true);
    boxSize += sizeof(number_of_key_samples);
    
    while (boxSize < boxHeader.size){
        uint32_t key_sample_id = Bytes2Int(data, data_size, sizeof(key_sample_id), true);
        boxSize += sizeof(key_sample_id);
        
        key_sample_id_tables.push_back(key_sample_id);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stssBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}

