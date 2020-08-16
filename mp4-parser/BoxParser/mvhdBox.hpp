//
//  mvhdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef mvhdBox_hpp
#define mvhdBox_hpp

#include "FullBox.hpp"

class mvhdBox: public FullBox{
public:
    mvhdBox();
    ~mvhdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* mvhdBox_hpp */
