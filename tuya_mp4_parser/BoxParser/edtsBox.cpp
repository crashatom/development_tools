//
//  edtsBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "edtsBox.hpp"
#include "BoxFactory.hpp"

edtsBox::edtsBox(){
    
}

edtsBox::~edtsBox(){
    
}

BasicBox *edtsBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new edtsBox;
    
    return box;
}

int edtsBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = 0;
    
    boxHeader.size = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(boxHeader.size);
    
    boxHeader.type = Bytes2String(data, 4, data_size, true);
    boxSize += boxHeader.type.length();
    
    data += boxHeader.size - boxSize;
    data_size -= (boxHeader.size - boxSize);
    
    boxSize += (boxHeader.size - boxSize);
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int edtsBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
