//
//  main.cpp
//  tuya_mp4_parser
//
//  Created by Jim on 2019/12/16.
//  Copyright © 2019 Tuya. All rights reserved.
//

#include <iostream>

#include "Mp4File.h"

int main(int argc, const char * argv[]) {
    char *path = (char *)"/Users/jim/Desktop/tuya_8k_2.mp4";
    
    Mp4File *mp4File = new Mp4File;
    
    mp4File->LoadFile(path);
    
    mp4File->PrintOverviewOfVideoFrames(32);
    
    
    return 0;
}

