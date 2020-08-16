//
//  stblBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stblBox.hpp"
#include "BoxFactory.hpp"

stblBox::stblBox(){
    
}

stblBox::~stblBox(){
    
}

BasicBox *stblBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stblBox;
    
    return box;
}

int stblBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = 0;
    
    boxHeader.size = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(boxHeader.size);
    
    boxHeader.type = Bytes2String(data, 4, data_size, true);
    boxSize += boxHeader.type.length();
    
    while (boxSize < boxHeader.size){
        unsigned char *ptr = data + 4;
        size_t size = data_size - 4;
        
        std::string boxName = Bytes2String(ptr, 4, size, false);
        
        BasicBox *box = BoxFactory::CreateBox(boxName);
        
        boxSize += box->ParseData(data, data_size);
        
        subBoxes.push_back(box);
    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stblBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
