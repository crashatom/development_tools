//
//  Mp4File.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include <algorithm>

#include "Mp4File.h"
#include "BoxFactory.hpp"

Mp4File::Mp4File(){
    
}

Mp4File::~Mp4File(){
    
}

int Mp4File::LoadFile(const char *path){
    if (path == nullptr)
        return ErrCode_ParamInvalid;
    
    FILE *file = fopen(path, "r");
    if (file == nullptr){
        printf("no file found on path:%s!\n", path);
        
        return ErrCode_ParamInvalid;
    }
    
    fseek(file, 0, SEEK_END);
    int fileSize = (int)ftell(file);
    fseek(file, 0, SEEK_SET);
    
    uint8_t *data = new uint8_t[fileSize];
    fread(data, 1, fileSize, file);
    
    return ParseData(data, fileSize);
}

BasicBox *Mp4File::GetBoxFromData(uint8_t *data, size_t &dataSize){
    if (dataSize < 8) return nullptr;
    
    uint8_t *tmp = data + 4;
    size_t tmpSize = dataSize - 4;
    std::string boxName = Bytes2String(tmp, 4, tmpSize, false);
    
    return BoxFactory::GetInstance()->CreateBox(boxName);
}

int Mp4File::ParseData(uint8_t *data, size_t data_size){
    if (data == nullptr)
        return ErrCode_DataInvalid;
    
    fileData = data;
    
    size_t data_left_size = data_size;
    uint8_t *ptr = data;
    
    while (data_left_size > 0){
        BasicBox *box = GetBoxFromData(ptr, data_left_size);
        
        if (box != nullptr){
            int nRetCode = box->ParseData(ptr, data_left_size);
            
            box->Describe();
        }
        
        boxes.push_back(box);
    }
    
    return 0;
}

int Mp4File::PrintOverviewOfVideoFrames(int maxNumberOfBytesToPrintPerFrame){
    // tuya
    stcoBox *_stcoBox = (stcoBox *)(boxes[3]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[2]->subBoxes[5]);
    stscBox *_stscBox = (stscBox *)(boxes[3]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[2]->subBoxes[3]);
    stszBox *_stszBox = (stszBox *)(boxes[3]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[2]->subBoxes[4]);
    sttsBox *_sttsBox = (sttsBox *)(boxes[3]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[2]->subBoxes[1]);
    
    int duration = 0;
    for (int i = 0; i < _sttsBox->number_of_entries; i++){
        duration += _sttsBox->time_to_sample_table[i].sample_duration;
    }
    
    
    // gaosi
//    stcoBox *_stcoBox = (stcoBox *)(boxes[3]->subBoxes[1]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[4]);
//    stscBox *_stscBox = (stscBox *)(boxes[3]->subBoxes[1]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[2]);
//    stszBox *_stszBox = (stszBox *)(boxes[3]->subBoxes[1]->subBoxes[1]->subBoxes[2]->subBoxes[2]->subBoxes[3]);
    
    int sample_index = 0;
    int chunk_count = _stcoBox->num_of_entries;
    for (int i = 0; i < chunk_count; i++){
        long chunk_offset = _stcoBox->chunkOffsets[i];
        int chunk_id = i + 1;
        
        uint8_t *chunk = fileData + chunk_offset;
        int samples_num_for_this_chunk = -1;
        
        for (int k = 0; k < _stscBox->num_of_entries; k++){
            if (_stscBox->sample_to_chunk_table[k].first_chunk_id > chunk_id){
                samples_num_for_this_chunk = _stscBox->sample_to_chunk_table[k - 1].samples_per_chunk;
                break;
            }
        }
        if (samples_num_for_this_chunk == -1){
            samples_num_for_this_chunk = _stscBox->sample_to_chunk_table[_stscBox->num_of_entries - 1].samples_per_chunk;
        }
        
        printf("chunk %02d {\n", chunk_id);
        
        uint8_t *sample = chunk;
        for (int index = 0; index < samples_num_for_this_chunk; index++, sample_index++){
            int sample_size = _stszBox->sample_size_table[sample_index];
            
            int print_bytes_num = std::min(maxNumberOfBytesToPrintPerFrame, sample_size);
            printf("    sample %02d: ", sample_index+1);
            for (int m = 0; m < print_bytes_num; m++){
                printf("0x%02x ", sample[m]);
            }
            printf("\n");
            
            sample += sample_size;
        }
        printf("}\n");
    }
    
    return 0;
}
