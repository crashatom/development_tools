//
//  ftypBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "BasicBox.h"
#include "ftypBox.hpp"

ftypBox::ftypBox(){
    
}

ftypBox::~ftypBox(){
    
}

BasicBox *ftypBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new ftypBox;
    
    return box;
}

int ftypBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = 0;
    
    boxHeader.size = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(boxHeader.size);
    
    boxHeader.type = Bytes2String(data, 4, data_size, true);
    boxSize += boxHeader.type.length();
    
    major_brand = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(major_brand);
    
    minor_version = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(minor_version);
    
    while(boxSize < boxHeader.size){
        std::string compatible_brand = Bytes2String(data, 4, data_size, true);
        boxSize += 4;
        
        compatible_brands.push_back(compatible_brand);
    }
    
    if (boxSize != boxHeader.size){
        assert(false);
    }
    
    return boxSize;
}

int ftypBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    printf("major_brand\t\t\t: %d\n", major_brand);
    printf("minor_version\t\t: %d\n", minor_version);
    printf("compatible_brands\t: ");
    for (int i = 0; i < compatible_brands.size(); i++){
        printf("%s ", compatible_brands[i].c_str());
    }
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
