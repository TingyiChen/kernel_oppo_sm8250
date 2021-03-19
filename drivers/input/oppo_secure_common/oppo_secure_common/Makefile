DEFS_PLATFORM = -DCONFIG_OPPO_BSP_SECCOM_PLATFORM=8250

ifneq ($(TARGET_PRODUCT),qssi)
ccflags-y += $(DEFS_PLATFORM)
obj-y     +=  oppo_secure_common.o
endif

