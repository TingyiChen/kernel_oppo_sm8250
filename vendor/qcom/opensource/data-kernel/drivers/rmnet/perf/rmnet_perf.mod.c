#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe1237f4e, "module_layout" },
	{ 0xb742f178, "kmalloc_caches" },
	{ 0x1fdc7df2, "_mcount" },
	{ 0xaa6b6624, "rmnet_get_port" },
	{ 0x6c07ef16, "set_normalized_timespec" },
	{ 0x98cf60b3, "strlen" },
	{ 0x8a43c967, "qmi_rmnet_ps_ind_deregister" },
	{ 0x4bd88d66, "napi_gro_flush" },
	{ 0x755e252f, "rmnet_get_endpoint" },
	{ 0x9f3691a8, "rmnet_frag_deliver" },
	{ 0xf2014711, "_raw_spin_lock_bh" },
	{ 0x43954257, "rmnet_map_flow_command" },
	{ 0xd2da1048, "register_netdevice_notifier" },
	{ 0xaecd47a, "param_ops_bool" },
	{ 0x8bdfe163, "__tracepoint_rmnet_perf_low" },
	{ 0xebb7200f, "qmi_rmnet_work_maybe_restart" },
	{ 0x7a2af7b4, "cpu_number" },
	{ 0xb385e2c9, "rmnet_recycle_frag_descriptor" },
	{ 0xc3e2d5a2, "kfree_skb_list" },
	{ 0x40295b0b, "rmnet_deliver_skb" },
	{ 0xe320a9d2, "rcu_read_lock_held" },
	{ 0x9d0d6206, "unregister_netdevice_notifier" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x4d65cbd5, "csum_ipv6_magic" },
	{ 0x73660017, "get_current_napi_context" },
	{ 0xdcb764ad, "memset" },
	{ 0x892da873, "debug_lockdep_rcu_enabled" },
	{ 0x7c32d0f0, "printk" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x449ad0a7, "memcmp" },
	{ 0x16624d6e, "__cpu_online_mask" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x4a71394e, "rmnet_perf_deag_entry" },
	{ 0xc7a0ce06, "rmnet_map_checksum_downlink_packet" },
	{ 0x5a921311, "strncmp" },
	{ 0x5792f848, "strlcpy" },
	{ 0x3128eeb0, "skb_push" },
	{ 0x8312a9d5, "skb_pull" },
	{ 0xf70e4a4d, "preempt_schedule_notrace" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0x16a42703, "rcu_read_lock_sched_held" },
	{ 0x4ea5d10, "ksize" },
	{ 0x4b87f955, "rmnet_perf_chain_end" },
	{ 0xb1e69621, "__alloc_skb" },
	{ 0xc5f200b0, "ipv6_skip_exthdr" },
	{ 0xb0426677, "_raw_spin_unlock_bh" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x6badccbb, "rmnet_frag_ipv6_skip_exthdr" },
	{ 0xe256f429, "rmnet_perf_desc_entry" },
	{ 0xa202a8e5, "kmalloc_order_trace" },
	{ 0x6fe3cb6d, "kfree_skb" },
	{ 0xbc008245, "rmnet_map_dl_ind_deregister" },
	{ 0xd25bc5d4, "csum_tcpudp_nofold" },
	{ 0x2ba31352, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x4829a47e, "memcpy" },
	{ 0x11e5634a, "param_array_ops" },
	{ 0x7171cf27, "qmi_rmnet_ps_ind_register" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc1e7a762, "rmnet_set_skb_proto" },
	{ 0x56bd2a08, "skb_put" },
	{ 0xbc79feec, "param_ops_ulong" },
	{ 0x92e95b34, "rmnet_map_dl_ind_register" },
	{ 0x7e9e325a, "param_ops_uint" },
	{ 0x3d5ee240, "__put_page" },
	{ 0xfe16ccff, "rmnet_is_real_dev_registered" },
	{ 0xae0bd7af, "lockdep_rcu_suspicious" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

