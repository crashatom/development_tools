//
//  tkhdBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef tkhdBox_hpp
#define tkhdBox_hpp

#include "FullBox.hpp"

class tkhdBox: public FullBox{
public:
    tkhdBox();
    ~tkhdBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* tkhdBox_hpp */
