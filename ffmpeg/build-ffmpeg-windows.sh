#!/bin/sh

# directories
SOURCE="ffmpeg-4.1.4"
FAT="ffmpeg-windows"
LIPO=`pwd`/"result"
SCRATCH="scratch"
# must be an absolute path
THIN=`pwd`/"thin"

rm -rf ./scratch
rm -rf ./thin

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

ARCHS="x86_64"

CWD=`pwd`
for ARCH in $ARCHS
do
	echo "building $ARCH..."
	mkdir -p "$SCRATCH/$ARCH"
	cd "$SCRATCH/$ARCH"

	CFLAGS="-arch $ARCH"	#-fembed-bitcode
	if [ "$ARCH" = "i386" -o "$ARCH" = "x86_64" ]
	then
	    
	fi

	echo "CFLAGS=$CFLAGS"
	echo "LDFLAGS=$LDFLAGS"

	TMPDIR=${TMPDIR/%\/} $CWD/$SOURCE/configure \
	    # --target-os=darwin \
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

echo Done
