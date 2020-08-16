//
//  dinfBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef dinfBox_hpp
#define dinfBox_hpp

#include "FullBox.hpp"

class dinfBox: public BasicBox{
public:
    dinfBox();
    ~dinfBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* dinfBox_hpp */
