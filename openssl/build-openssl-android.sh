#!/bin/sh

set -u

OPENSSL_VERSION=1.1.1c
SOURCE="$0"
# BUILD_TARGETS=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64")
BUILD_TARGETS=("armeabi" "armeabi-v7a" "arm64-v8a")
API_LEVELS=(19 19 21)
export ANDROID_NDK=/Users/jim/Library/Android/sdk/android-ndk-r17c

# 
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
PWDPATH="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

# 
kernelName="$(uname -s)"
case "${kernelName}" in
    Linux*)     MACHINE=Linux;;
    Darwin*)    MACHINE=Mac;;
    CYGWIN*)    MACHINE=Cygwin;;
    MINGW*)     MACHINE=MinGw;;
    *)          MACHINE="UNKNOWN:${kernelName}"
esac

# 
BUILD_DIR=$PWDPATH/tmp
OUT_DIR=$PWDPATH/built-result-android

# openssl
if [ ! -d openssl-${OPENSSL_VERSION} ]
then
    if [ ! -f openssl-${OPENSSL_VERSION}.tar.gz ]
    then
        wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz || exit 128
    fi
    tar xzf openssl-${OPENSSL_VERSION}.tar.gz || exit 128
fi

# openssl
cd openssl-${OPENSSL_VERSION} || exit 128

# 
build_the_thing() {
    if [[ "$MACHINE" == "Mac" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/darwin-x86_64
    elif [[ "$MACHINE" == "Linux" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64
    fi
    export PATH=$TOOLCHAIN/$TRIBLE/bin:$TOOLCHAIN/bin:"$PATH"
    
		echo $PATH

    make clean
    ./Configure $SSL_TARGET $OPTIONS zlib no-asm no-shared no-unit-test && \
    make && \
    make install DESTDIR=$DESTDIR || exit 128
}


for ((i=0; i < ${#BUILD_TARGETS[@]}; i++))
do
		BUILD_TARGET=${BUILD_TARGETS[i]}
		ANDROID_API_LEVEL=${API_LEVELS[i]}

    case $BUILD_TARGET in
    armeabi)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        OPTIONS="--target=armv5te-linux-androideabi -mthumb -fPIC -latomic -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        DESTDIR="$BUILD_DIR/armeabi"
        SSL_TARGET="android-arm"
    ;;
    armeabi-v7a)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        OPTIONS="--target=armv7a-linux-androideabi -Wl,--fix-cortex-a8 -fPIC -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        DESTDIR="$BUILD_DIR/armeabi-v7a"
        SSL_TARGET="android-arm"
    ;;
    arm64-v8a)
        TRIBLE="aarch64-linux-android"
        TC_NAME="aarch64-linux-android-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        DESTDIR="$BUILD_DIR/arm64-v8a"
        SSL_TARGET="android-arm64"
    ;;
    x86)
        TRIBLE="i686-linux-android"
        TC_NAME="x86-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        DESTDIR="$BUILD_DIR/x86"
        SSL_TARGET="android-x86"
    ;;
    x86_64)
        TRIBLE="x86_64-linux-android"
        TC_NAME="x86_64-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        DESTDIR="$BUILD_DIR/x86_64"
        SSL_TARGET="android-x86_64"
    ;;
    esac
    
    rm -rf $DESTDIR
    build_the_thing
    
		#### copy libraries and includes to output-directory #####
    mkdir -p $OUT_DIR/$BUILD_TARGET/include
    cp -R $DESTDIR/usr/local/include/* $OUT_DIR/$BUILD_TARGET/include
    mkdir -p $OUT_DIR/$BUILD_TARGET/lib
    cp -R $DESTDIR/usr/local/lib/*.so $OUT_DIR/$BUILD_TARGET/lib
    cp -R $DESTDIR/usr/local/lib/*.a $OUT_DIR/$BUILD_TARGET/lib
done

echo success
