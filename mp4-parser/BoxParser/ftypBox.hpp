//
//  ftypBox.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef ftypBox_hpp
#define ftypBox_hpp

#include "BasicBox.h"

// aligned(8) class FileTypeBox extends Box(‘ftyp’)
// {
//     unsigned int(32) major_brand;
//     unsigned int(32) minor_version;
//     unsigned int(32) compatible_brands[];  // to end of the box
// }

class ftypBox: public BasicBox{
public:
    ftypBox();
    ~ftypBox();
    
    int ParseData(uint8_t *&data, size_t &data_size);
    static BasicBox *CreateBox(std::string boxName);
    int Describe();
    
public:
    
    uint32_t major_brand;
    uint32_t minor_version;
    std::vector<std::string> compatible_brands;
};

#endif /* ftypBox_hpp */
