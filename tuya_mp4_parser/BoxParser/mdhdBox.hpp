//
//  mdhdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef mdhdBox_hpp
#define mdhdBox_hpp

#include "FullBox.hpp"

class mdhdBox: public BasicBox{
public:
    mdhdBox();
    ~mdhdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* mdhdBox_hpp */
