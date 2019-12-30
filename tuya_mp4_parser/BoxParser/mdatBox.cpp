//
//  mdatBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "mdatBox.hpp"

mdatBox::mdatBox(){
    
}

mdatBox::~mdatBox(){
    
}

int mdatBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = 0;
    
    boxHeader.size = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(boxHeader.size);
    
    boxHeader.type = Bytes2String(data, 4, data_size, true);
    boxSize += boxHeader.type.length();
    
    data += boxHeader.size - boxSize;
    data_size -= (boxHeader.size - boxSize);
    
//    while(boxSize < boxHeader.size){
//        Sample *sample = new Sample;
//
//        sample->size = Bytes2Int(data, data_size, 4, true);
//        boxSize += sizeof(sample->size);
//        data_size -= sizeof(sample->size);
//
//        sample->data = new uint8_t[sample->size];
//
//        if (sample->data == nullptr){
//            return ErrCode_NoMemory;
//        }
//
//        memcpy(sample->data, data, sample->size);
//        boxSize += sample->size;
//
//        printf("%x %x %x %x %x\n", data[0], data[1], data[2], data[3], data[4]);
//
//        samples.push_back(sample);
//
//        data += sample->size;
//        data_size -= sample->size;
//    }
    
//    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

BasicBox *mdatBox::CreateBox(std::string boxName){
    return new mdatBox;
}

int mdatBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    printf("samples:\n");
    for (int i = 0; i < samples.size(); i++){
        printf("sample%d: size:%zu, first 4 bytes:", i, samples[i]->size);
        
        for (int k = 0; k < 4; k++){
            printf("%x ", samples[i]->data[k]);
        }
        printf("\n");
    }
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}


