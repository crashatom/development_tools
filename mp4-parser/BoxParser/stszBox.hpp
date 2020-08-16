//
//  stszBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stszBox_hpp
#define stszBox_hpp

#include <vector>

#include "FullBox.hpp"

class stszBox: public FullBox{
    struct sample_entry{
        uint32_t xxx;
        
    };
    
public:
    stszBox();
    ~stszBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
    
public:
    uint32_t sample_size;
    uint32_t number_of_entries;
    
    std::vector<uint32_t> sample_size_table;
};


#endif /* stszBox_hpp */
