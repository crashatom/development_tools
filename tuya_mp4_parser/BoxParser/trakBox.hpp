//
//  trakBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef trakBox_hpp
#define trakBox_hpp

#include "FullBox.hpp"

class trakBox: public BasicBox{
public:
    trakBox();
    ~trakBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* trakBox_hpp */
