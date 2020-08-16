//
//  stcoBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/17.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stcoBox_hpp
#define stcoBox_hpp

#include "FullBox.hpp"

class stcoBox: public FullBox{
public:
    stcoBox();
    ~stcoBox();
    
    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    uint32_t num_of_entries;
    std::vector<uint32_t> chunkOffsets; // in this file
};

#endif /* stcoBox_hpp */
