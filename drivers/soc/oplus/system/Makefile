#
# Makefile for the OPLUS Linux Kernel system team specific device drivers.
#

obj-y 	+= platform.o

obj-$(CONFIG_OPLUS_FEATURE_SHUTDOWN_DETECT)    += shutdown_detect/
obj-$(CONFIG_OPLUS_FEATURE_OPROJECT) 	+= oplus_project/

obj-$(CONFIG_OPLUS_FEATURE_DUMP_DEVICE_INFO)	+=  dump_device_info/


obj-$(CONFIG_OPLUS_FEATURE_PROJECTINFO)	+=  project_info/

#ifdef OPLUS_FEATURE_ESIM
#ChenGuoyao@NETWORK.SIM,2020/06/10, Add for eSIM
obj-$(CONFIG_OPLUS_OPPO_GPIO)    +=  oppo_gpio/
#endif /* OPLUS_FEATURE_ESIM */
