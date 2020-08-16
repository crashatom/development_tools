//
//  edtsBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef edtsBox_hpp
#define edtsBox_hpp

#include "FullBox.hpp"

class edtsBox: public BasicBox{
public:
    edtsBox();
    ~edtsBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* edtsBox_hpp */
