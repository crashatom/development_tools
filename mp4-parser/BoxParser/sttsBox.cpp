//
//  sttsBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "sttsBox.hpp"

sttsBox::sttsBox(){
    
}

sttsBox::~sttsBox(){
    
}

BasicBox *sttsBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new sttsBox;
    
    return box;
}

int sttsBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    number_of_entries = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(number_of_entries);
    
    while(boxSize < boxHeader.size){
        time_to_sample_table_entry entry = {0};
        
        entry.sample_count = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(entry.sample_count);
        
        entry.sample_duration = Bytes2Int(data, data_size, 4, true);
        boxSize += sizeof(entry.sample_duration);
        
        time_to_sample_table.push_back(entry);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int sttsBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
