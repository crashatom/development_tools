//
//  mdiaBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef mdiaBox_hpp
#define mdiaBox_hpp

#include "FullBox.hpp"

class mdiaBox: public BasicBox{
public:
    mdiaBox();
    ~mdiaBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* mdiaBox_hpp */
