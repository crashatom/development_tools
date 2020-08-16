//
//  stsdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stsdBox_hpp
#define stsdBox_hpp

#include "FullBox.hpp"

class stsdBox: public FullBox{
    struct st_video_media_sample_info{
        uint16_t version;
        uint16_t revision_level;
        uint32_t vendor;
        uint32_t temporal_quality;
        uint32_t spatial_quality;
        uint16_t width;
        uint16_t height;
        uint32_t horizontal_resolution;
        uint32_t vertical_resolution;
        uint32_t data_size;
        uint16_t frame_count;
        uint8_t  compressor_name[32];
        uint16_t color_depth;
        uint16_t color_table_id;
    };
    
    struct st_optional_description_atoms{
        uint32_t size;
        std::string type;
        
        uint8_t *data;
    };
    
    struct st_sample_description{
        uint32_t sample_description_size;
        uint8_t  data_format[4];
        uint16_t reserved0;
        uint16_t reserved1;
        uint16_t reserved2;
        uint16_t data_reference_index;
        
        st_video_media_sample_info video_media_sample_info;
        st_optional_description_atoms optional_description_atoms;
    };
    
public:
    stsdBox();
    ~stsdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    uint32_t number_of_entries;
    
    std::vector<st_sample_description> sample_description_table;
};

#endif /* stsdBox_hpp */
