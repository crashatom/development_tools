//
//  BoxFactory.hpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#ifndef BoxFactory_hpp
#define BoxFactory_hpp

#include <map>
#include <string>
#include <stdio.h>

#include "BasicBox.h"

#include "ftypBox.hpp"
#include "freeBox.hpp"
#include "mdatBox.hpp"
#include "moovBox.hpp"
#include "mvhdBox.hpp"
#include "trakBox.hpp"
#include "tkhdBox.hpp"
#include "edtsBox.hpp"
#include "mdiaBox.hpp"
#include "mdhdBox.hpp"
#include "hdlrBox.hpp"
#include "minfBox.hpp"
#include "vmhdBox.hpp"
#include "smhdBox.hpp"
#include "dinfBox.hpp"
#include "stblBox.hpp"
#include "stsdBox.hpp"
#include "sttsBox.hpp"
#include "stssBox.hpp"
#include "stscBox.hpp"
#include "stszBox.hpp"
#include "stcoBox.hpp"
#include "udtaBox.hpp"
#include "BoxFactory.hpp"

typedef BasicBox *(*FUNC_BOX_CREATE)(std::string boxName);

class BoxFactory{
private:
    class gc{
    public:
        gc(){}
        ~gc(){
            if (self){
                delete self;
                self = NULL;
            }
        }
    };
    static gc m_sGC;
private:
    static BoxFactory *self;
    
public:
    static BoxFactory *GetInstance(){
        return self;
    }
    
public:
    BoxFactory();
    
public:
    static BasicBox *CreateBox(std::string boxName);
    
private:
    static std::map<std::string, FUNC_BOX_CREATE> mMapBoxCreatorfuncs;
    
};

#endif /* BoxFactory_hpp */
