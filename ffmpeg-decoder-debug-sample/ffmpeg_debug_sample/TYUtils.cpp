#include <string>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <sys/time.h>

#include "TYUtils.h"
#include "TYPublic.h"
#include <string.h>
#include <algorithm>
#include <sstream>

bool ty_is_big_endian(){
    union NUM{
        int  a;
        char b;
    }num;
    
    num.a = 0x1234;
    
    if (num.b == 0x12){
        return true;
    }
    
    return false;
}

long long GetCurrentMSTime(){
    struct timeval tv = {0};

    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

const char *SimpleEncodeNumber2Letter(long num){
    static char letters[32] = {0};
    int index = 0;
    
    while (num) {
        int a = num % 10;
        num /= 10;
        
        letters[index ++] = a + 'a';
    }
    
    return letters;
}

int ms_delay(long delay_ms){
    struct timeval tv = {0};
    struct timespec req, rem;
    long total_nano_delay = delay_ms*1000000;
    long nano_delay = 0;
    int ret = 0;

    gettimeofday(&tv, NULL);
    long long endtime = 0;
    long long starttime = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    while (total_nano_delay > 0){
        nano_delay = 100000;
        total_nano_delay -= 100000;

        while(nano_delay > 0)
        {
            rem.tv_sec = 0;
            rem.tv_nsec = 0;
            req.tv_sec = 0;
            req.tv_nsec = nano_delay;
            nanosleep(&req, &rem);

            nano_delay = rem.tv_nsec;
        };

        gettimeofday(&tv, NULL);
        endtime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        if (endtime - starttime >= delay_ms)
            break;
    }

    return ret;
}
unsigned char *find_nalu_sei(unsigned char *stream,int size)
{
    
    return NULL ;
}

int parse_nalu_sei(unsigned char * data, int size,char **uuid,int &uuidSize,char **sei,int &seiSize){
    
    return 0;
}

int get_point_pos_(char *seiInfo,int seiLen,objectRect **objs_,int &objlen) {
    
    return 0;
}

void draw_one_frame_yuv420(unsigned char *y,unsigned char *u,unsigned char *v,int width,int height,objectRect objs) {
    
}
