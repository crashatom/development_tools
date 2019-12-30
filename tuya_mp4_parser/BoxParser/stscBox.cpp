//
//  stscBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stscBox.hpp"

stscBox::stscBox(){
    
}

stscBox::~stscBox(){
    
}

BasicBox *stscBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stscBox;
    
    return box;
}

int stscBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    num_of_entries = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(num_of_entries);
    
    while(boxSize < boxHeader.size){
        chunks_block_entry entry = {0};
        
        entry.first_chunk_id = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(entry.first_chunk_id);
        
        entry.samples_per_chunk = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(entry.samples_per_chunk);
        
        entry.sample_description_id = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(entry.sample_description_id);
        
        sample_to_chunk_table.push_back(entry);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stscBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}

