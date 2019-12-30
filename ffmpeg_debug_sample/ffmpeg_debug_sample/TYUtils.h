//
//  TYUtils.h
//  TYCameraSdk
//
//  Created by Jim on 2018/3/9.
//  Copyright © 2018年 TuyaSmart. All rights reserved.
//

#ifndef TYUtils_h
#define TYUtils_h

#include <string>
#define UUID_SIZE                   16
#define FRAME_HEIGHT                4
typedef struct _objectRect__
{
    int topx,topy,botx,boty;
}objectRect;


int parse_nalu_sei(unsigned char * data, int size,char **uuid,int &uuidSize,char **sei,int &seiSize) ;
unsigned char *find_nalu_sei(unsigned char *stream,int size)   ;        // return the start pointer of sei nalu
int get_point_pos_(char *seiInfo,int seiLen,objectRect **objs_,int &objlen) ;
void draw_one_frame_yuv420(unsigned char *y,unsigned char *u,unsigned char *v,int width,int height,objectRect objs) ;


#endif // TYUtils_h
