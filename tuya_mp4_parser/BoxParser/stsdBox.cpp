//
//  stsdBox.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "stsdBox.hpp"

stsdBox::stsdBox(){
    
}

stsdBox::~stsdBox(){
    
}

BasicBox *stsdBox::CreateBox(std::string boxName){
    BasicBox *box = (BasicBox *)new stsdBox;
    
    return box;
}

int stsdBox::ParseData(uint8_t *&data, size_t &data_size){
    int boxSize = FullBox::ParseData(data, data_size);
    
    number_of_entries = Bytes2Int(data, data_size, 4, true);
    boxSize += sizeof(number_of_entries);
    
    data += boxHeader.size - boxSize;
    data_size -= (boxHeader.size - boxSize);
    
    boxSize += (boxHeader.size - boxSize);
    
//    while (boxSize < boxHeader.size) {
//        st_sample_description sample_description = {0};
//
//        sample_description.sample_description_size = Bytes2Int(data, data_size, sizeof(sample_description.sample_description_size), true);
//        boxSize += sizeof(sample_description.sample_description_size);
//
//        memcpy(sample_description.data_format, data, sizeof(sample_description.data_format));
//        data += sizeof(sample_description.data_format);
//        data_size -= sizeof(sample_description.data_format);
//        boxSize += sizeof(sample_description.data_format);
//
//        sample_description.reserved0 = Bytes2Int(data, data_size, sizeof(sample_description.reserved0), true);
//        boxSize += sizeof(sample_description.reserved0);
//        sample_description.reserved1 = Bytes2Int(data, data_size, sizeof(sample_description.reserved1), true);
//        boxSize += sizeof(sample_description.reserved1);
//        sample_description.reserved2 = Bytes2Int(data, data_size, sizeof(sample_description.reserved2), true);
//        boxSize += sizeof(sample_description.reserved2);
//
//        sample_description.data_reference_index = Bytes2Int(data, data_size, sizeof(sample_description.data_reference_index), true);
//        boxSize += sizeof(sample_description.data_reference_index);
//        sample_description.video_media_sample_info.version = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.version), true);
//        boxSize += sizeof(sample_description.data_reference_index);
//        sample_description.video_media_sample_info.revision_level = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.revision_level), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.revision_level);
//        sample_description.video_media_sample_info.vendor = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.vendor), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.vendor);
//        sample_description.video_media_sample_info.temporal_quality = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.temporal_quality), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.temporal_quality);
//        sample_description.video_media_sample_info.spatial_quality = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.spatial_quality), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.spatial_quality);
//        sample_description.video_media_sample_info.width = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.width), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.width);
//        sample_description.video_media_sample_info.height = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.height), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.height);
//
//        sample_description.video_media_sample_info.horizontal_resolution = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.horizontal_resolution), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.horizontal_resolution);
//
//        sample_description.video_media_sample_info.vertical_resolution = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.vertical_resolution), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.vertical_resolution);
//
//        sample_description.video_media_sample_info.data_size = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.data_size), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.data_size);
//
//        sample_description.video_media_sample_info.frame_count = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.frame_count), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.frame_count);
//
//        memcpy(sample_description.video_media_sample_info.compressor_name, data, sizeof(sample_description.video_media_sample_info.compressor_name));
//        data += sizeof(sample_description.video_media_sample_info.compressor_name);
//        boxSize += sizeof(sample_description.video_media_sample_info.compressor_name);
//        data_size -= sizeof(sample_description.video_media_sample_info.compressor_name);
//
//        sample_description.video_media_sample_info.color_depth = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.color_depth), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.color_depth);
//
//        sample_description.video_media_sample_info.color_table_id = Bytes2Int(data, data_size, sizeof(sample_description.video_media_sample_info.color_table_id), true);
//        boxSize += sizeof(sample_description.video_media_sample_info.color_table_id);
//
//        sample_description.optional_description_atoms.size = Bytes2Int(data, data_size, sizeof(sample_description.optional_description_atoms.size), true);
//        boxSize += sizeof(sample_description.optional_description_atoms.size);
//        sample_description.optional_description_atoms.type = Bytes2String(data, 4, data_size, true);
//        boxSize += sample_description.optional_description_atoms.type.length();
//
//        sample_description.optional_description_atoms.data = new uint8_t[boxHeader.size - boxSize];
//        memcpy(sample_description.optional_description_atoms.data, data, boxHeader.size - boxSize);
//        data += boxHeader.size - boxSize;
//        boxSize += boxHeader.size - boxSize;
//        data_size -= boxHeader.size - boxSize;
//
//        sample_description_table.push_back(sample_description);
//    }
    
    assert(boxSize == boxHeader.size);
    
    return boxSize;
}

int stsdBox::Describe(){
    printf("==========================================\n");
    printf("size\t\t\t\t: %d(bytes)\n", boxHeader.size);
    printf("type\t\t\t\t: %s\n", boxHeader.type.c_str());
    
    printf("\n");
    printf("==========================================\n\n");
    
    return ErrCode_NoErr;
}
