//
//  smhdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef smhdBox_hpp
#define smhdBox_hpp

#include "FullBox.hpp"

class smhdBox: public FullBox{
public:
    smhdBox();
    ~smhdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* smhdBox_hpp */
