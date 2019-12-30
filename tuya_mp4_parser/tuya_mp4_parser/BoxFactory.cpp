//
//  BoxFactory.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include "BoxFactory.hpp"

#define FTYP_BOX "ftyp"
#define FREE_BOX "free"
#define MDAT_BOX "mdat"
#define MOOV_BOX "moov"
#define MVHD_BOX "mvhd"
#define TRAK_BOX "trak"
#define TKHD_BOX "tkhd"
#define EDTS_BOX "edts"
#define MDIA_BOX "mdia"
#define MDHD_BOX "mdhd"
#define HDLR_BOX "hdlr"
#define MINF_BOX "minf"
#define VMHD_BOX "vmhd"
#define SMHD_BOX "smhd"
#define DINF_BOX "dinf"
#define STBL_BOX "stbl"
#define STSD_BOX "stsd"
#define STTS_BOX "stts"
#define STSS_BOX "stss"
#define STSC_BOX "stsc"
#define STSZ_BOX "stsz"
#define STCO_BOX "stco"
#define UDTA_BOX "udta"


std::map<std::string, FUNC_BOX_CREATE> BoxFactory::mMapBoxCreatorfuncs = {};
BoxFactory *BoxFactory::self = new BoxFactory;

BoxFactory::BoxFactory(){
    mMapBoxCreatorfuncs[FTYP_BOX] = ftypBox::CreateBox;
    mMapBoxCreatorfuncs[FREE_BOX] = freeBox::CreateBox;
    mMapBoxCreatorfuncs[MDAT_BOX] = mdatBox::CreateBox;
    mMapBoxCreatorfuncs[MOOV_BOX] = moovBox::CreateBox;
    mMapBoxCreatorfuncs[MVHD_BOX] = mvhdBox::CreateBox;
    mMapBoxCreatorfuncs[TRAK_BOX] = trakBox::CreateBox;
    mMapBoxCreatorfuncs[TKHD_BOX] = tkhdBox::CreateBox;
    mMapBoxCreatorfuncs[EDTS_BOX] = edtsBox::CreateBox;
    mMapBoxCreatorfuncs[MDIA_BOX] = mdiaBox::CreateBox;
    mMapBoxCreatorfuncs[MDHD_BOX] = mdhdBox::CreateBox;
    mMapBoxCreatorfuncs[HDLR_BOX] = hdlrBox::CreateBox;
    mMapBoxCreatorfuncs[MINF_BOX] = minfBox::CreateBox;
    mMapBoxCreatorfuncs[VMHD_BOX] = vmhdBox::CreateBox;
    mMapBoxCreatorfuncs[SMHD_BOX] = smhdBox::CreateBox;
    mMapBoxCreatorfuncs[DINF_BOX] = dinfBox::CreateBox;
    mMapBoxCreatorfuncs[STBL_BOX] = stblBox::CreateBox;
    mMapBoxCreatorfuncs[STSD_BOX] = stsdBox::CreateBox;
    mMapBoxCreatorfuncs[STTS_BOX] = sttsBox::CreateBox;
    mMapBoxCreatorfuncs[STSS_BOX] = stssBox::CreateBox;
    mMapBoxCreatorfuncs[STSC_BOX] = stscBox::CreateBox;
    mMapBoxCreatorfuncs[STSZ_BOX] = stszBox::CreateBox;
    mMapBoxCreatorfuncs[STCO_BOX] = stcoBox::CreateBox;
    mMapBoxCreatorfuncs[UDTA_BOX] = udtaBox::CreateBox;
    
}

BasicBox *BoxFactory::CreateBox(std::string boxName){
    std::map<std::string, FUNC_BOX_CREATE>::iterator it = mMapBoxCreatorfuncs.find(boxName);
    
    if (it != mMapBoxCreatorfuncs.end()){
        return it->second(boxName);
    }else{
        char msg[256] = {0};
        sprintf(msg, "unknown box: %s!", boxName.c_str());
        printf("%s\n", msg);
        
        assert(false);
    }
    
    return nullptr;
}
