#!/bin/sh

export IPHONEOS_DEPLOYMENT_TARGET="8.0"
# Catalina 系统使用gcc 编译，clang编译报socket()找不到
export CC=/Applications/Xcode.app/Contents/Developer/usr/bin/gcc

LIB_NAME="curl-7.70.0"

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

OUTDIR=${pwd_path}/built-result-ios

function build(){
	pushd .; cd "${LIB_NAME}";

	ARCH=$1
	HOST=$2
	SDKDIR=$3
	
	export CFLAGS="-arch ${ARCH} -pipe -Os -gdwarf-2 -isysroot ${SDKDIR} -I${SDKDIR} -I${SDKDIR}/usr/include -miphoneos-version-min=${IPHONEOS_DEPLOYMENT_TARGET}" # -fembed-bitcode
	export LDFLAGS="-arch ${ARCH} -isysroot ${SDKDIR}"
	
	./configure --host="${HOST}-apple-darwin" --with-darwinssl --enable-ipv6 --disable-shared --enable-static --disable-smtp --disable-pop3 --disable-imap --disable-ftp --disable-tftp --disable-telnet --disable-rtsp --disable-ldap
	
	make clean
	make -j `sysctl -n hw.logicalcpu_max`
	cp lib/.libs/libcurl.a $OUTDIR/libcurl-${ARCH}.a
	
	popd
}

 mkdir -p $OUTDIR

 #build armv7   armv7   `xcrun --sdk iphoneos --show-sdk-path`
 ##build armv7s  armv7s  `xcrun --sdk iphoneos --show-sdk-path`
 #build arm64    arm    `xcrun --sdk iphoneos --show-sdk-path`
 #build i386    i386   `xcrun --sdk iphonesimulator --show-sdk-path`
 #build x86_64  x86_64  `xcrun --sdk iphonesimulator --show-sdk-path`

 lipo -arch armv7 $OUTDIR/libcurl-armv7.a \
    -arch arm64 $OUTDIR/libcurl-arm64.a \
    -arch i386 $OUTDIR/libcurl-i386.a \
    -arch x86_64 $OUTDIR/libcurl-x86_64.a \
    -create -output $OUTDIR/libcurl_all.a

 ls $OUTDIR