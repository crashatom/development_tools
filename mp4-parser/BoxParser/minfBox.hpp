//
//  minfBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef minfBox_hpp
#define minfBox_hpp

#include "FullBox.hpp"

class minfBox: public BasicBox{
public:
    minfBox();
    ~minfBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* minfBox_hpp */
