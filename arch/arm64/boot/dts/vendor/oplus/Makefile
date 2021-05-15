ifeq ($(CONFIG_BUILD_ARM64_DT_OVERLAY),y)
    dtbo-$(CONFIG_ARCH_LITO) += cherry-mtp-overlay.dtbo \
                    cherry-v2-mtp-overlay.dtbo

    cherry-mtp-overlay.dtbo-base := lito.dtb
    cherry-v2-mtp-overlay.dtbo-base := lito-v2.dtb
else
    dtb-$(CONFIG_ARCH_LITO) += lito-mtp.dtb
endif

always		:= $(dtb-y)
subdir-y	:= $(dts-dirs)
clean-files    := *.dtb *.dtbo

