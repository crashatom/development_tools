//
//  stblBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef stblBox_hpp
#define stblBox_hpp

#include "FullBox.hpp"

class stblBox: public BasicBox{
public:
    stblBox();
    ~stblBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* stblBox_hpp */
