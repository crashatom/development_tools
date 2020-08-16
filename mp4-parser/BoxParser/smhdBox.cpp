//
//  smhdBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "smhdBox.hpp"

smhdBox::smhdBox(){
    
}

smhdBox::~smhdBox(){
    
}

BasicBox *smhdBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new smhdBox;
    
    return box;
}

int smhdBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    data += boxHeader.size - boxSize;
    data_size -= (boxHeader.size - boxSize);
    
    boxSize += (boxHeader.size - boxSize);
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int smhdBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
