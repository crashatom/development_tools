//
//  freeBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef freeBox_hpp
#define freeBox_hpp

#include "BasicBox.h"

class freeBox: public BasicBox{
public:
    freeBox(){};
    ~freeBox(){};
    
    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    
};

#endif /* freeBox_hpp */
