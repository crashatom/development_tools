//
//  udtaBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef udtaBox_hpp
#define udtaBox_hpp

#include "BasicBox.h"

class udtaBox: public BasicBox{
public:
    udtaBox(){};
    ~udtaBox(){};
    
    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    
};

#endif /* udtaBox_hpp */
