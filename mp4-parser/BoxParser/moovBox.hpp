//
//  moovBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef moovBox_hpp
#define moovBox_hpp

#include "FullBox.hpp"

class moovBox: public BasicBox{    
public:
    moovBox();
    ~moovBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* moovBox_hpp */
