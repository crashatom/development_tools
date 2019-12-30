#!/bin/sh

# directories
SOURCE="ffmpeg-4.1.4"
FAT="FFmpeg-iOS"
LIPO=`pwd`/"result"
SCRATCH="scratch"
# must be an absolute path
THIN=`pwd`/"thin"

rm -rf ./scratch
rm -rf ./thin

# absolute path to x264 libraryx264
#X264=`pwd`/../x264/x264-iOS

#FDK_AAC=`pwd`/fdk-aac-ios
#CONFIGURE_FLAG_FFMPEG="--enable-avresample --enable-demuxers --enable-muxers --disable-ffplay \
#--disable-ffmpeg --disable-ffprobe --disable-neon --enable-libx264 --enable-encoder=libx264 --enable-decoder=h264 \
#--enable-protocols --enable-hwaccels --enable-zlib --disable-devices --enable-avdevice"


#CONFIGURE_FLAG_FFMPEG="--disable-gpl \
#--disable-neon \
#--disable-nonfree \
#--disable-gray \
#--disable-swscale-alpha \
#--disable-ffmpeg \
#--disable-debug \
#--disable-doc \
#--disable-ffplay \
#--disable-ffprobe \
#--disable-htmlpages \
#--disable-manpages \
#--disable-podpages \
#--disable-txtpages \
#--disable-avdevice \
#--disable-doc \
#--disable-d3d11va \
#--disable-dxva2 \
#--disable-vaapi \
#--disable-vdpau \
#--enable-videotoolbox \
#--disable-symver \
#--disable-encoders  \
#--disable-muxers \
#--disable-demuxers \
#--disable-parsers  \
#--disable-bsfs \
#--disable-protocols \
#--enable-hwaccels \
#--disable-indevs \
#--disable-outdevs \
#--disable-filters \
#--disable-decoders \
#--enable-version3 \
#--enable-shared \
#--enable-nonfree \
#--enable-gpl \
#--enable-avformat \
#--disable-avresample \
#--enable-avcodec \
#--enable-avutil \
#--disable-swresample \
#--disable-swscale \
#--disable-avfilter \
#--disable-postproc \
#--enable-parser=h264 \
#--enable-decoder=h264 \
#--enable-protocol=file \
#--enable-protocol=rtmp \
#--enable-muxer=rawvideo  \
#--enable-encoder=rawvideo  \
#--enable-network \
#--enable-decoder=aac_latm \
#--enable-decoder=aac \
#--enable-muxer=mp4 \
#--enable-demuxer=hls \
#--enable-parser=aac \
#--enable-parser=h264 \
#--enable-runtime-cpudetect"


#CONFIGURE_FLAG_FFMPEG="
#--disable-programs --disable-ffmpeg --disable-ffprobe --disable-ffplay \
#--disable-everything \
#--disable-avdevice --disable-swresample --disable-postproc --enable-avfilter --enable-swscale \
#--disable-network \
#--disable-symver \
#--disable-bsfs \
#--disable-devices --disable-indevs --disable-outdevs \
#--disable-filters \
#--disable-encoders --enable-encoder=mjpeg --enable-encoder=aac --enable-encoder=libfdk_aac \
#--disable-decoders --enable-decoder=h264 --enable-decoder=aac --enable-decoder=libfdk_aac \
#--disable-muxers --enable-muxer=mp4 --enable-muxer=h264 --enable-muxer=mjpeg \
#--disable-demuxers --enable-demuxer=h264 --enable-demuxer=aac --enable-demuxer=mpegvideo --enable-demuxer=mov \
#--disable-parsers --enable-parser=mpeg4video --enable-parser=aac --enable-parser=h264 \
#--disable-protocols --enable-protocol=file"

sudo xcode-select -s /Applications/Xcode-9.1.app/Contents/Developer

# --enable-debug --disable-optimizations --disable-stripping --disable-inline-asm \

CONFIGURE_FLAG_FFMPEG="
--enable-cross-compile --disable-debug --enable-stripping --enable-static --disable-doc --enable-pic --enable-small \
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


CONFIGURE_FLAGS=""

if [ "$X264" ]
then
	CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-gpl --enable-libx264"
fi

if [ "$FDK_AAC" ]
then
	CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-libfdk-aac --enable-gpl --enable-nonfree"
fi


echo "CONFIGURE_FLAGS====>$CONFIGURE_FLAGS"

# avresample
#CONFIGURE_FLAGS="$CONFIGURE_FLAGS --enable-avresample"

ARCHS="arm64 armv7 x86_64 i386"
#ARCHS="arm64"
#ARCHS="arm64"
COMPILE="y"
LIPO="y"

DEPLOYMENT_TARGET="8.0"

if [ "$*" ]
then
	if [ "$*" = "lipo" ]
	then
		# skip compile
		COMPILE=
	else
		ARCHS="$*"
		if [ $# -eq 1 ]
		then
			# skip lipo
			LIPO=
		fi
	fi
fi

if [ "$COMPILE" ]
then
	if [ ! `which yasm` ]
	then
		echo 'Yasm not found'
		if [ ! `which brew` ]
		then
			echo 'Homebrew not found. Trying to install...'
                        ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" \
				|| exit 1
		fi
		echo 'Trying to install Yasm...'
		brew install yasm || exit 1
	fi
	if [ ! `which gas-preprocessor.pl` ]
	then
		echo 'gas-preprocessor.pl not found. Trying to install...'
		(curl -L https://github.com/libav/gas-preprocessor/raw/master/gas-preprocessor.pl \
			-o /usr/local/bin/gas-preprocessor.pl \
			&& chmod +x /usr/local/bin/gas-preprocessor.pl) \
			|| exit 1
	fi

	if [ ! -r $SOURCE ]
	then
		echo 'FFmpeg source not found. Trying to download...'
		curl http://www.ffmpeg.org/releases/$SOURCE.tar.bz2 | tar xj \
			|| exit 1
	fi

	CWD=`pwd`
	for ARCH in $ARCHS
	do
		echo "building $ARCH..."
		mkdir -p "$SCRATCH/$ARCH"
		cd "$SCRATCH/$ARCH"

		CFLAGS="-arch $ARCH"	#-fembed-bitcode
		if [ "$ARCH" = "i386" -o "$ARCH" = "x86_64" ]
		then
		    PLATFORM="iPhoneSimulator"
		    CFLAGS="$CFLAGS -mios-simulator-version-min=$DEPLOYMENT_TARGET"
		else
		    PLATFORM="iPhoneOS"
		    CFLAGS="$CFLAGS -mios-version-min=$DEPLOYMENT_TARGET"
		    if [ "$ARCH" = "arm64" ]
		    then
		        EXPORT="GASPP_FIX_XCODE5=1"
		    fi
		fi

		XCRUN_SDK=`echo $PLATFORM | tr '[:upper:]' '[:lower:]'`
		CC="xcrun -sdk $XCRUN_SDK clang"
		CXXFLAGS="$CFLAGS"
		LDFLAGS="$CFLAGS"
		if [ "$X264" ]
		then
			CFLAGS="$CFLAGS -I$X264/include"
			LDFLAGS="$LDFLAGS -L$X264/lib"
		fi
		if [ "$FDK_AAC" ]
		then
			CFLAGS="$CFLAGS -I$FDK_AAC/include"
			LDFLAGS="$LDFLAGS -L$FDK_AAC/lib"
		fi

		echo "CFLAGS=$CFLAGS"
		echo "LDFLAGS=$LDFLAGS"


		TMPDIR=${TMPDIR/%\/} $CWD/$SOURCE/configure \
		    --target-os=darwin \
		    --arch=$ARCH \
		    --cc="$CC" \
		    $CONFIGURE_FLAGS \
		    $CONFIGURE_FLAG_FFMPEG \
		    --extra-cflags="$CFLAGS" \
		    --extra-cxxflags="$CXXFLAGS" \
		    --extra-ldflags="$LDFLAGS" \
		    --prefix="$THIN/$ARCH" \
		|| exit 1

		make -j3 install $EXPORT || exit 1
		cd $CWD
	done
fi

if [ "$LIPO" ]
then
	echo "building fat binaries..."
	mkdir -p $FAT/lib
	set - $ARCHS
	CWD=`pwd`
	cd $THIN/$1/lib
	for LIB in *.a
	do
		cd $CWD
		echo lipo -create `find $THIN -name $LIB` -output $FAT/lib/$LIB 1>&2
		lipo -create `find $THIN -name $LIB` -output $FAT/lib/$LIB || exit 1
	done

	cd $CWD
	cp -rf $THIN/$1/include $FAT
fi

echo Done
