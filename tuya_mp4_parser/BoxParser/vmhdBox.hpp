//
//  vmhdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef vmhdBox_hpp
#define vmhdBox_hpp

#include "FullBox.hpp"

class vmhdBox: public FullBox{
public:
    vmhdBox();
    ~vmhdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* vmhdBox_hpp */
