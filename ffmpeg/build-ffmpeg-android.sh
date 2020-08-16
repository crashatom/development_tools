#!/bin/sh

SOURCE="ffmpeg-4.1.4"
PREFIX=android-build

# 4.1.4 r17c
#NDK_HOME=/Users/jim/Library/Android/sdk/android-ndk-r17c
#NDK_HOME=/Users/jim/Library/Android/sdk/android-ndk-r11
NDK_HOME=/Users/jim/Library/Android/sdk/ndk-bundle
NDK_HOST_PLATFORM=darwin-x86_64
export TMPDIR="/Users/jim/Git/development_tools/ffmpeg/tmp"
SCRIPT_PATH=`pwd`
#cd ${SCRIPT_PATH}/${SOURCE}

#COMMON_OPTIONS="\
#    --prefix=android/ \
#    --target-os=android \
#    --enable-cross-compile --disable-debug --disable-doc --enable-pic --enable-small \
#		--enable-shared \
#		--disable-static \
#		--disable-programs --disable-ffmpeg --disable-ffprobe --disable-ffplay \
#		--disable-everything \
#		--disable-avdevice --enable-swresample --disable-postproc --enable-avfilter --enable-swscale \
#		--disable-network \
#		--disable-symver \
#		--disable-bsfs \
#		--disable-devices --disable-indevs --disable-outdevs \
#		--disable-filters \
#		--disable-encoders --enable-encoder=mjpeg --enable-encoder=aac \
#		--disable-decoders --enable-decoder=h264 --enable-decoder=aac --enable-decoder=hevc \
#		--disable-muxers --enable-muxer=mp4 --enable-muxer=h264 --enable-muxer=mjpeg \
#		--disable-demuxers --enable-demuxer=h264 --enable-demuxer=aac --enable-demuxer=mpegvideo --enable-demuxer=mov \
#		--disable-parsers --enable-parser=mpeg4video --enable-parser=aac --enable-parser=h264 --enable-parser=mjpeg \
#		--disable-protocols --enable-protocol=file"
           

# armeabi api level19


COMMON_OPTIONS="--prefix=android/ \
    --target-os=android \
    --enable-cross-compile --disable-debug --disable-doc --enable-pic --enable-small --disable-neon \
		--enable-shared \
		--enable-static \
		--disable-asm \
		--disable-inline-asm \
		--disable-programs --disable-ffmpeg --disable-ffprobe --enable-ffplay \
		--disable-everything \
		--disable-avdevice --enable-swresample --disable-postproc --enable-avfilter --enable-swscale \
		--enable-network \
		--disable-symver \
		--disable-bsfs \
		--disable-devices --disable-indevs --disable-outdevs \
		--disable-filters \
		--disable-encoders --enable-encoder=mjpeg --enable-encoder=aac \
		--disable-decoders --enable-decoder=h264 --enable-decoder=aac --enable-decoder=hevc \
		--disable-muxers --enable-muxer=mp4 --enable-muxer=h264 --enable-muxer=hevc --enable-muxer=mjpeg --enable-muxer=adts --enable-muxer=rtsp \
		--disable-demuxers --enable-demuxer=h264 --enable-demuxer=hevc --enable-demuxer=aac --enable-demuxer=mpegvideo --enable-demuxer=mov --enable-demuxer=rtsp --enable-demuxer=sdp --enable-demuxer=flv \
		--disable-parsers --enable-parser=mpeg4video --enable-parser=aac --enable-parser=h264 --enable-parser=hevc \
		--disable-protocols --enable-protocol=file --enable-protocol=tcp --enable-protocol=rtp --enable-protocol=rtmp"
           
           
function build_android {
#		./configure \
#    --libdir=${PREFIX}/libs/armeabi \
#    --incdir=${PREFIX}/includes/armeabi \
#    --pkgconfigdir=${PREFIX}/pkgconfig/armeabi \
#    --arch=arm \
#    --cpu=armv6 \
#    --cross-prefix="${NDK_HOME}/toolchains/arm-linux-androideabi-4.9/prebuilt/${NDK_HOST_PLATFORM}/bin/arm-linux-androideabi-" \
#    --sysroot="${NDK_HOME}/platforms/android-19/arch-arm/" \
#    --extra-cflags="-march=armv6 -mfloat-abi=softfp -mfpu=vfp -I$NDK_HOME/sysroot/usr/include/arm-linux-androideabi -isysroot $NDK_HOME/sysroot" \
#    --extra-ldexeflags=-pie \
#    ${COMMON_OPTIONS}
#    make clean
#    make -j8 && make install
#
#		./configure \
#    --libdir=${PREFIX}/libs/armeabi-v7a \
#    --incdir=${PREFIX}/includes/armeabi-v7a \
#    --pkgconfigdir=${PREFIX}/pkgconfig/armeabi-v7a \
#    --arch=arm \
#    --cpu=armv7-a \
#    --cross-prefix="${NDK_HOME}/toolchains/arm-linux-androideabi-4.9/prebuilt/${NDK_HOST_PLATFORM}/bin/arm-linux-androideabi-" \
#    --sysroot="${NDK_HOME}/platforms/android-19/arch-arm/" \
#    --extra-cflags="-march=armv7-a -mfloat-abi=softfp -mfpu=neon -I$NDK_HOME/sysroot/usr/include/arm-linux-androideabi -isysroot $NDK_HOME/sysroot" \
#    --extra-ldexeflags=-pie \
#    ${COMMON_OPTIONS}
#    make clean
#    make -j8 && make install

#    ./configure \
#    --libdir=${PREFIX}/libs/arm64-v8a \
#    --incdir=${PREFIX}/includes/arm64-v8a \
#    --pkgconfigdir=${PREFIX}/pkgconfig/arm64-v8a \
#    --arch=aarch64 \
#    --cpu=armv8-a \
#    --cross-prefix="${NDK_HOME}/toolchains/aarch64-linux-android-4.9/prebuilt/${NDK_HOST_PLATFORM}/bin/aarch64-linux-android-" \
#    --sysroot="${NDK_HOME}/platforms/android-21/arch-arm64/" \
#    --extra-cflags="-march=armv8-a -isysroot ${NDK_HOME}/sysroot -U_FILE_OFFSET_BITS -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated" \
#    --extra-ldflags="-marm" \
#    --extra-ldexeflags=-pie \
#    ${COMMON_OPTIONS}
#    make clean
#    make -j8 && make install
                
                   

    ./configure \
    --libdir=${PREFIX}/libs/arm64-v8a \
    --incdir=${PREFIX}/includes/arm64-v8a \
    --pkgconfigdir=${PREFIX}/pkgconfig/arm64-v8a \
    --arch=aarch64 \
    --cross-prefix="${NDK_HOME}/toolchains/llvm/prebuilt/${NDK_HOST_PLATFORM}/bin/aarch64-linux-android-" \
    --sysroot="${NDK_HOME}/platforms/android-21/arch-arm64/" \
    --extra-cflags="-D__ANDROID_API__=21 -Os -fPIC -DANDROID" \
    --extra-ldexeflags=-pie \
    ${COMMON_OPTIONS} 
    make clean
    make -j8 
    make install  
                    
                    
                    # -fPIC -DANDROID -D__thumb__ -mthumb -Wfatal-errors -Wno-deprecated
                    
                    
                    
                    
                    
                    
                    
                    
#    ./configure \
#    --libdir=${PREFIX}/libs/x86 \
#    --incdir=${PREFIX}/includes/x86 \
#    --pkgconfigdir=${PREFIX}/pkgconfig/x86 \
#    --arch=x86 \
#    --cpu=i686 \
#    --cross-prefix="${NDK_HOME}/toolchains/x86-4.9/prebuilt/${NDK_HOST_PLATFORM}/bin/i686-linux-android-" \
#    --sysroot="${NDK_HOME}/platforms/android-16/arch-x86/" \
#    --extra-ldexeflags=-pie \
#    ${COMMON_OPTIONS} 
#    make clean
#    make -j8 && make install

#    ./configure \
#    --libdir=${PREFIX}/libs/x86_64 \
#    --incdir=${PREFIX}/includes/x86_64 \
#    --pkgconfigdir=${PREFIX}/pkgconfig/x86_64 \
#    --arch=x86_64 \
#    --cpu=x86_64 \
#    --cross-prefix="${NDK_HOME}/toolchains/x86_64-4.9/prebuilt/${NDK_HOST_PLATFORM}/bin/x86_64-linux-android-" \
#    --sysroot="${NDK_HOME}/platforms/android-21/arch-x86_64/" \
#    --extra-ldexeflags=-pie \
#    ${COMMON_OPTIONS}
#    make clean
#    make -j8 && make install
};

build_android

cd ${SCRIPT_PATH}