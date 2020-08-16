#!/bin/bash

set -u

TOOLS_ROOT=`pwd`

SOURCE="$0"
while [ -h "$SOURCE" ]; do
    DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
done
pwd_path="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

echo pwd_path=${pwd_path}
echo TOOLS_ROOT=${TOOLS_ROOT}
# read -n1 -p "Press any key to continue..."

# Setting
IOS_MIN_TARGET="8.0"
LIB_NAME="openssl-1.1.1c"
LIB_DEST_DIR="${pwd_path}/../built-result-ios/fat"
HEADER_DEST_DIR="include"

# ARCHS=("arm64" "armv7s" "armv7" "i386" "x86_64")
# SDKS=("iphoneos" "iphoneos" "iphoneos" "iphonesimulator" "iphonesimulator")
# PLATFORMS=("iPhoneOS" "iPhoneOS" "iPhoneOS" "iPhoneSimulator" "iPhoneSimulator")

ARCHS=("arm64" "armv7s" "armv7" "x86_64")
SDKS=("iphoneos" "iphoneos" "iphoneos" "iphonesimulator")
PLATFORMS=("iPhoneOS" "iPhoneOS" "iPhoneOS" "iPhoneSimulator")

# ARCHS=("arm64")
# SDKS=("iphoneos")
# PLATFORMS=("iPhoneOS")

DEVELOPER=`xcode-select -print-path`
SDK_VERSION=`xcrun -sdk iphoneos --show-sdk-version`
rm -rf "${HEADER_DEST_DIR}" "${LIB_DEST_DIR}" "${LIB_NAME}"
[ -f "${LIB_NAME}.tar.gz" ] || curl https://www.openssl.org/source/${LIB_NAME}.tar.gz > ${LIB_NAME}.tar.gz
 
# Unarchive library, then configure and make for specified architectures
configure_make()
{
   ARCH=$1; SDK=$2; PLATFORM=$3;
   if [ -d "${LIB_NAME}" ]; then
       rm -fr "${LIB_NAME}"
   fi
   tar xfz "${LIB_NAME}.tar.gz"
   pushd .; cd "${LIB_NAME}";

   if [[ "${ARCH}" == "i386" || "${ARCH}" == "x86_64" ]]; then
       echo ""
   else
       sed -ie "s!static volatile sig_atomic_t intr_signal;!static volatile intr_signal;!" "crypto/ui/ui_openssl.c"
   fi

   export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
   export CROSS_SDK="${PLATFORM}${SDK_VERSION}.sdk"
   export TOOLS="${DEVELOPER}"
   export CC="${TOOLS}/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -arch ${ARCH}"

   PREFIX_DIR="${pwd_path}/../output/ios/openssl-${ARCH}"
   if [ -d "${PREFIX_DIR}" ]; then
       rm -fr "${PREFIX_DIR}"
   fi
   mkdir -p "${PREFIX_DIR}"

   if [[ "${ARCH}" == "x86_64" ]]; then
        unset IPHONEOS_DEPLOYMENT_TARGET
       ./Configure darwin64-x86_64-cc --prefix="${PREFIX_DIR}" "-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}"
   elif [[ "${ARCH}" == "i386" ]]; then
        unset IPHONEOS_DEPLOYMENT_TARGET
       ./Configure darwin-i386-cc --prefix="${PREFIX_DIR}" "-isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK}"
   else
        # instruction: 
        # 1.no-shared and -fembed-bitcode is not compatibility
        # 2.advise use only .a static library on iOS
       export IPHONEOS_DEPLOYMENT_TARGET=${IOS_MIN_TARGET}
       ./Configure iphoneos-cross no-shared --prefix="${PREFIX_DIR}" "-fembed-bitcode"
   fi
   
	 read -n1 -p "Press any key to continue..."
   
   if [ ! -d ${CROSS_TOP}/SDKs/${CROSS_SDK} ]; then
       echo "ERROR: iOS SDK version:'${SDK_VERSION}' incorrect, SDK in your system is:"
       xcodebuild -showsdks | grep iOS
       exit -1
   fi
   
   OUTPUT_ROOT=${TOOLS_ROOT}/../output/ios/openssl-${ARCH}
   mkdir -p ${OUTPUT_ROOT}/log
   make clean &> "${OUTPUT_ROOT}/log/${ARCH}.log"
   if make -j8 >> "${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
   then
       # make install;
       make install_sw >> "${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1
       make install_ssldirs >> "${OUTPUT_ROOT}/log/${ARCH}.log" 2>&1      
   fi

   popd
}

for ((i=0; i < ${#ARCHS[@]}; i++))
do
    if [[ $# -eq 0 || "$1" == "${ARCHS[i]}" ]]; then
        configure_make "${ARCHS[i]}" "${SDKS[i]}" "${PLATFORMS[i]}"
    fi
done

# Combine libraries for different architectures into one
# Use .a files from the temp directory by providing relative paths
create_lib()
{
   LIB_SRC=$1; LIB_DST=$2;
   LIB_PATHS=( "${ARCHS[@]/#/${pwd_path}/../output/ios/openssl-}" )
   LIB_PATHS=( "${LIB_PATHS[@]/%//lib/${LIB_SRC}}" )
   lipo ${LIB_PATHS[@]} -create -output "${LIB_DST}"
}
mkdir -p "${LIB_DEST_DIR}";
create_lib "libcrypto.a" "${LIB_DEST_DIR}/libcrypto.a"
create_lib "libssl.a" "${LIB_DEST_DIR}/libssl.a"