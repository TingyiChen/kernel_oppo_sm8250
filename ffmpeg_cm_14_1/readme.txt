��ǰĿ¼�µ�ffmpeg source code ��Ҫ�������룬��Ϊ��android Q �ı��뻷���£�����ffmpeg����ʾ��������Ĵ���


ld.lld: error: can't create dynamic relocation R_ARM_V4BX against local symbol in readonly segment; recompile object files with -fPIC or pass '-Wl,-z,notext' to allow text relocations in the output
>>> defined in out/target/product/sdm710/obj_arm/SHARED_LIBRARIES/libavutil_vendor_intermediates/arm/float_dsp_neon.o
>>> referenced by out/target/product/sdm710/obj_arm/SHARED_LIBRARIES/libavutil_vendor_intermediates/arm/float_dsp_neon.o:(ff_vector_fmul_reverse_neon)

���Բο������վ��˵����
https://android.googlesource.com/platform/bionic/+/master/android-changes-for-ndk-developers.md#Text-Relocations-Enforced-for-API-level-23

������ʾ �����ʱ��ʹ�� -Wl,-z,notext ����Ա���ͨ�� ��������dlopen ��ص�so ��ʱ�� ��ʧ�� ����ʾso ������ relocations text������ relocations text
�Ĵ��� �ĺ��� ��ff_vector_fmul_reverse_neon ������� ��һ������ļ� �޷������޸�Ϊ����relocations text �Ĵ��롣

Note: it is technically possible to have a shared object with the TEXTREL entry/flag but without any actual text relocations. This doesn��t happen with the NDK,
=>���������ʾ ����ʹ��NDK �ı��뻷�� ���ԡ�

Ŀǰʹ��NDK �ı��뻷�����������so �����������С�

���Ե�ǰĿ¼��source ��Ҫ����NDK �ı��뻷����NDK�ı��뻷���������������æ���ã����ߵ���ý��SVN ��ȥ���أ���ý��SVN\NDK\ndk.tar.gz

��ѹ�����NDK ������ ��Ҫ��build_ffmpeg_android.sh �ļ����޸����NDK ��������

NDK=/work/mm_dashuang.zhang/ndk/android-ndk-r10e

�޸�Ϊ���Լ������á�

