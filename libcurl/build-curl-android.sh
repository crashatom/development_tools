#!/bin/sh

set -u

LIBCURL_VERSION=7.70.0
LIBCURL_NAME="curl-${LIBCURL_VERSION}"
SOURCE="$0"
# BUILD_TARGETS=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64")
BUILD_TARGETS=("arm64-v8a")
API_LEVELS=(21)
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


# libcurl
if [ ! -d curl-${LIBCURL_VERSION} ]
then
    if [ ! -f curl-${LIBCURL_VERSION}.tar.gz ]
    then
        wget https://curl.haxx.se/download/curl-${LIBCURL_VERSION}.tar.gz || exit 128
    fi
    tar xzf curl-${LIBCURL_VERSION}.tar.gz || exit 128
fi

# libcurl
cd curl-${LIBCURL_VERSION} || exit 128

# 
build_the_thing() {
    if [[ "$MACHINE" == "Mac" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/darwin-x86_64
    elif [[ "$MACHINE" == "Linux" ]]; then
        TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64
    fi
    export PATH=$TOOLCHAIN/$TRIBLE/bin:$TOOLCHAIN/bin:"$PATH"
    
		echo $PATH

		CPPFLAGS="-I/Users/jim/Git/ipc-client-development-tools/openssl/built-result-android/${BUILD_TARGET}/include $OPTIONS  -std=c++11 -frtti -fexceptions"
		LDFLAGS="-L/Users/jim/Git/ipc-client-development-tools/openssl/built-result-android/${BUILD_TARGET}/lib ${ARCH_LINK}"


		SYSROOT=${ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot

		#export PKG_CONFIG_PATH=/Users/jim/Git/ipc-client-development-tools/openssl/built-result-android/armeabi-v7a/lib/pkgconfig

		# ssl 配置方式参考官方文档: https://curl.haxx.se/docs/install.html
    ./configure --prefix=${BUILD_DIR}/${BUILD_TARGET} --with-sysroot=${SYSROOT} --host=${TRIBLE} \
    --enable-static \
    --disable-shared \
    --enable-threaded-resolver \
    --enable-ipv6 && \
    make clean && \
    make && \
    make install || exit 128
}

for ((i=0; i < ${#BUILD_TARGETS[@]}; i++))
do
		BUILD_TARGET=${BUILD_TARGETS[i]}
		ANDROID_API_LEVEL=${API_LEVELS[i]}

    case $BUILD_TARGET in
    armeabi)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        OPTIONS="-fPIC -mthumb"
        ARCH_LINK=""
        DESTDIR="$BUILD_DIR/armeabi"
        SSL_TARGET="android-arm"
    ;;
    armeabi-v7a)
        TRIBLE="arm-linux-androideabi"
        TC_NAME="arm-linux-androideabi-4.9"
        OPTIONS="-fPIC -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon"
        ARCH_LINK="-march=armv7-a -Wl,--fix-cortex-a8"
        DESTDIR="$BUILD_DIR/armeabi-v7a"
        SSL_TARGET="android-arm"
    ;;
    arm64-v8a)
        TRIBLE="aarch64-linux-android"
        TC_NAME="aarch64-linux-android-4.9"
        OPTIONS="-fPIC"
        ARCH_LINK=""
        DESTDIR="$BUILD_DIR/arm64-v8a"
        SSL_TARGET="android-arm64"
    ;;
    x86)
        TRIBLE="i686-linux-android"
        TC_NAME="x86-4.9"
        OPTIONS="-fPIC -march=i686 -mtune=intel -msse3 -mfpmath=sse -m32"
        ARCH_LINK=""
        DESTDIR="$BUILD_DIR/x86"
        SSL_TARGET="android-x86"
    ;;
    x86_64)
        TRIBLE="x86_64-linux-android"
        TC_NAME="x86_64-4.9"
        OPTIONS="-fPIC -D__ANDROID_API__=${ANDROID_API_LEVEL}"
        ARCH_LINK=""
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
