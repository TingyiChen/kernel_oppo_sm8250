#!/bin/bash
# ��������ndk ���û���������ȷ��NDK ������Ŀǰֻ��֧��android-ndk-r10e�����ߵİ汾�޷��Զ���ͬʱ�����32λ��64λ����Ҫ���ֶ�ִ��./configure
NDK=/work/mm_dashuang.zhang/ndk/android-ndk-r10e

configure()
{
    CPU=$1
    PREFIX=$(pwd)/android/$CPU
    HOST=""
    CROSS_PREFIX=""
    SYSROOT=""
    ARCH=""
    if [ "$CPU" == "armv7-a" ]
    then
        ARCH="arm"
        HOST=arm-linux
        SYSROOT=$NDK/platforms/android-21/arch-arm/
        CROSS_PREFIX=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-
        ADDI_CFLAGS="-march=armv7-a -marm -mtune=cortex-a8 -mfloat-abi=softfp -mfpu=neon"
    else
        ARCH="aarch64"
        HOST=aarch64-linux
        SYSROOT=$NDK/platforms/android-21/arch-arm64/
        CROSS_PREFIX=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-
        ADDI_CFLAGS="-march=armv8-a"
    fi
    ./configure \
    --prefix=$PREFIX \
    --disable-static \
    --disable-ffplay \
    --disable-network \
    --disable-ffmpeg \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-doc \
    --disable-symver \
    --disable-postproc \
    --disable-avdevice \
    --disable-avfilter \
    --enable-neon \
    --enable-asm \
    --enable-shared \
    --enable-pic \
    --enable-cross-compile \
    --cross-prefix=$CROSS_PREFIX \
    --target-os=android \
    --arch=$ARCH \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fPIC -DANDROID $ADDI_CFLAGS" \
    --extra-ldflags=""
}

build()
{
    make clean
    cpu=$1
    echo "build $cpu"

    configure $cpu
    make -j32
    make install
    rm -rf $(pwd)/android/$CPU/lib/pkgconfig
    if [ "$CPU" == "armv7-a" ]
    then
        cp  $(pwd)/android/$CPU/lib/*.so  $(pwd)/prebuilt/lib/
    else
        cp  $(pwd)/android/$CPU/lib/*.so  $(pwd)/prebuilt/lib64/
    fi

    rm -rf $(pwd)/android/$CPU
    rm -rf $(pwd)/libavcodec/*.so
    rm -rf $(pwd)/libavcodec/*.o
    rm -rf $(pwd)/libavformat/*.so
    rm -rf $(pwd)/libavformat/*.o
    rm -rf $(pwd)/libswresample/*.so
    rm -rf $(pwd)/libswresample/*.o
    rm -rf $(pwd)/libswscale/*.so
    rm -rf $(pwd)/libswscale/*.o
    rm -rf $(pwd)/libavutil/*.so
    rm -rf $(pwd)/libavutil/*.o

}

build arm64
build armv7-a
