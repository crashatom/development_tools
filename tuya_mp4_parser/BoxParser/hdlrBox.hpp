//
//  hdlrBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/21.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef hdlrBox_hpp
#define hdlrBox_hpp

#include "FullBox.hpp"

class hdlrBox: public FullBox{
public:
    hdlrBox();
    ~hdlrBox();

    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
};

#endif /* hdlrBox_hpp */
