//
//  stscBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stscBox_hpp
#define stscBox_hpp

#include "FullBox.hpp"

class stscBox: public FullBox{
    struct chunks_block_entry{
        uint32_t first_chunk_id;
        uint32_t samples_per_chunk;
        uint32_t sample_description_id;
    };
    
public:
    stscBox();
    ~stscBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
    
public:
    uint32_t num_of_entries;
    
    std::vector<chunks_block_entry> sample_to_chunk_table;
};

#endif /* stscBox_hpp */
