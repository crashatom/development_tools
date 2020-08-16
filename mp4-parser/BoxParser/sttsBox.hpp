//
//  sttsBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef sttsBox_hpp
#define sttsBox_hpp

#include "FullBox.hpp"

class sttsBox: public FullBox{
    struct time_to_sample_table_entry{
        uint32_t sample_count;
        uint32_t sample_duration;
    };
    
public:
    sttsBox();
    ~sttsBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    uint32_t number_of_entries;
    
    std::vector<time_to_sample_table_entry> time_to_sample_table;
};

#endif /* sttsBox_hpp */
