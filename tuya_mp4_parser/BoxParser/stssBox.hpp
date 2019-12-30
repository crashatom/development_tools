//
//  stssBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stssBox_hpp
#define stssBox_hpp

#include "FullBox.hpp"

class stssBox: public FullBox{
public:
    stssBox();
    ~stssBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
    
public:
    uint32_t number_of_key_samples;
    std::vector<uint32_t> key_sample_id_tables;
};

#endif /* stssBox_hpp */
