cmd_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o := python /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/scripts/gcc-wrapper.py /work/oppo_80300754/find_x2_full/SM8250_Q/android/vendor/qcom/proprietary/llvm-arm-toolchain-ship/8.0/bin/clang -Wp,-MD,/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/.rmnet_perf_udp_opt.o.d  -nostdinc -isystem /work/oppo_80300754/find_x2_full/SM8250_Q/android/vendor/qcom/proprietary/llvm-arm-toolchain-ship/8.0/lib/clang/8.0.12/include -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include -I./arch/arm64/include/generated  -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include -I./include -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi -I./arch/arm64/include/generated/uapi -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi -I./include/generated/uapi -include /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kconfig.h -include /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler_types.h  -I/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf -I/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf -D__KERNEL__ -DVENDOR_EDIT -mlittle-endian -Qunused-arguments -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -DVENDOR_EDIT --target=aarch64-linux-gnu --prefix=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/ --gcc-toolchain=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 -no-integrated-as -Werror=unknown-warning-option -fno-PIE -mno-implicit-float -DCONFIG_AS_LSE=1 -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -Wno-address-of-packed-member -O2 -Wframe-larger-than=2048 -fstack-protector-strong --target=aarch64-linux-gnu --gcc-toolchain=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 -meabi gnu -Wno-format-invalid-specifier -Wno-gnu -Wno-duplicate-decl-specifier -Wno-asm-operand-widths -Wno-initializer-overrides -fno-builtin -Wno-undefined-optimized -Wno-tautological-constant-out-of-range-compare -mllvm -disable-struct-const-merge -Wno-tautological-compare -mno-global-merge -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wvla -g -pg -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-merge-all-constants -fmerge-constants -fno-stack-check -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -Wno-initializer-overrides -Wno-unused-value -Wno-format -Wno-sign-compare -Wno-format-zero-length -Wno-uninitialized  -DMODULE  -DVENDOR_EDIT -DKBUILD_BASENAME='"rmnet_perf_udp_opt"' -DKBUILD_MODNAME='"rmnet_perf"' -c -o /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/.tmp_rmnet_perf_udp_opt.o /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.c

source_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o := /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.c

deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o := \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kconfig.h \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler-clang.h \
    $(wildcard include/config/cfi/clang.h) \
    $(wildcard include/config/lto/clang.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/hyperv/net.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/xps.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/bql.h) \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/rfs/accel.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/xfrm/offload.h) \
    $(wildcard include/config/tls/device.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/libfcoe.h) \
    $(wildcard include/config/wireless/ext.h) \
    $(wildcard include/config/net/switchdev.h) \
    $(wildcard include/config/net/l3/master/dev.h) \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/vlan/8021q.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/tipc.h) \
    $(wildcard include/config/irda.h) \
    $(wildcard include/config/atalk.h) \
    $(wildcard include/config/decnet.h) \
    $(wildcard include/config/mpls/routing.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/netfilter/ingress.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/garp.h) \
    $(wildcard include/config/mrp.h) \
    $(wildcard include/config/cgroup/net/prio.h) \
    $(wildcard include/config/net/flow/limit.h) \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/dynamic/debug.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timer.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/debug/objects/timers.h) \
    $(wildcard include/config/no/hz/common.h) \
    $(wildcard include/config/hotplug/cpu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/types.h \
  arch/arm64/include/generated/uapi/asm/types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/int-ll64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/int-ll64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/bitsperlong.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitsperlong.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/bitsperlong.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/posix_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stddef.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/stddef.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/posix_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/posix_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
    $(wildcard include/config/page/poisoning/zero.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/const.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/const.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/arch/has/refcount.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/tracing.h) \
  /work/oppo_80300754/find_x2_full/SM8250_Q/android/vendor/qcom/proprietary/llvm-arm-toolchain-ship/8.0/lib/clang/8.0.12/include/stdarg.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/linkage.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stringify.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/export.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/barrier.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/barrier.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kasan-checks.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/linkage.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bitops.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bits.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/bitops.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/builtin-__ffs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/builtin-ffs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/builtin-__fls.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/builtin-fls.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/ffz.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/fls64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/sched.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/hweight.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/arch_hweight.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/const_hweight.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/atomic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/atomic.h \
    $(wildcard include/config/arm64/lse/atomics.h) \
    $(wildcard include/config/as/lse.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/lse.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/atomic_ll_sc.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cmpxchg.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/build_bug.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/atomic-long.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/lock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/non-atomic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/le.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/byteorder.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/byteorder/little_endian.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/byteorder/little_endian.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/swab.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/swab.h \
  arch/arm64/include/generated/uapi/asm/swab.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/swab.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/byteorder/generic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/typecheck.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/quiet.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk/nmi.h) \
    $(wildcard include/config/printk.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/init.h \
    $(wildcard include/config/strict/kernel/rwx.h) \
    $(wildcard include/config/strict/module/rwx.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kern_levels.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/kernel.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sysinfo.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cache.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cputype.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/sysreg.h \
    $(wildcard include/config/broken/gas/inst.h) \
    $(wildcard include/config/arm64/pa/bits/52.h) \
    $(wildcard include/config/arm64/4k/pages.h) \
    $(wildcard include/config/arm64/16k/pages.h) \
    $(wildcard include/config/arm64/64k/pages.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/compiler.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dynamic_debug.h \
    $(wildcard include/config/jump/label.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ktime.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/seqlock.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/preempt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/trace/preempt/toggle.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  arch/arm64/include/generated/asm/preempt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/preempt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/thread_info.h \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/bug/on/data/corruption.h) \
    $(wildcard include/config/panic/on/data/corruption.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/bug.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/asm-bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/brk-imm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/restart_block.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/time64.h \
    $(wildcard include/config/64bit/time.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  arch/arm64/include/generated/asm/div64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/div64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/time.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/current.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/thread_info.h \
    $(wildcard include/config/arm64/sw/ttbr0/pan.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/memory.h \
    $(wildcard include/config/arm64/va/bits.h) \
    $(wildcard include/config/kasan/extra.h) \
    $(wildcard include/config/vmap/stack.h) \
    $(wildcard include/config/debug/align/rodata.h) \
    $(wildcard include/config/blk/dev/initrd.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/page-def.h \
    $(wildcard include/config/arm64/page/shift.h) \
    $(wildcard include/config/arm64/cont/shift.h) \
  arch/arm64/include/generated/asm/sizes.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/sizes.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sizes.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/vm/pgflags.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pfn.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/stack_pointer.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/irqflags.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/ptrace.h \
    $(wildcard include/config/compat.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/ptrace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/hwcap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/hwcap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/sigcontext.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/ptrace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bottom_half.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/spinlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/spinlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qspinlock_types.h \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/nr/cpus.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qrwlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stacktrace.h \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/user/stacktrace/support.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/spinlock.h \
  arch/arm64/include/generated/asm/qrwlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qrwlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/gcc/plugin/stackleak.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
    $(wildcard include/config/fortify/source.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/string.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/string.h \
    $(wildcard include/config/arch/has/uaccess/flushcache.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/alternative.h \
    $(wildcard include/config/arm64/uao.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cpucaps.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/insn.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cpufeature.h \
    $(wildcard include/config/arm64/sve.h) \
    $(wildcard include/config/arm64/ssbd.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/jump_label.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/hw_breakpoint.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/virt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/sections.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/sections.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/pgtable-hwdef.h \
    $(wildcard include/config/pgtable/levels.h) \
    $(wildcard include/config/arm64/pa/bits.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/fpsimd.h \
  arch/arm64/include/generated/uapi/asm/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/errno-base.h \
  arch/arm64/include/generated/asm/qspinlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qspinlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
    $(wildcard include/config/generic/lockbreak.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/time32.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/jiffies.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timex.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/timex.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/param.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/param.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/param.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/timex.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/arch_timer.h \
    $(wildcard include/config/arm/arch/timer/ool/workaround.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/smp.h \
    $(wildcard include/config/up/late/init.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/threads.h \
    $(wildcard include/config/base/small.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bitmap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/smp.h \
    $(wildcard include/config/arm64/acpi/parking/protocol.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/percpu.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
    $(wildcard include/config/virtualization.h) \
    $(wildcard include/config/amd/mem/encrypt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/clocksource/arm_arch_timer.h \
    $(wildcard include/config/arm/arch/timer.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timecounter.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/timex.h \
  include/generated/timeconst.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timekeeping.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timekeeping32.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/delay.h \
  arch/arm64/include/generated/asm/delay.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/delay.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/prefetch.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rculist.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcupdate.h \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/arch/weak/release/acquire.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcutree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dynamic_queue_limits.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ethtool.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compat.h \
    $(wildcard include/config/arch/has/syscall/wrapper.h) \
    $(wildcard include/config/x86/x32/abi.h) \
    $(wildcard include/config/compat/old/sigaction.h) \
    $(wildcard include/config/odd/rt/sigaction.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compat_time.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/compat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched.h \
    $(wildcard include/config/oppo/healthinfo.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/sched/info.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/sched/walt.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/process/reclaim/enhance.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/psi.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/memcg/kmem.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/stackprotector.h) \
    $(wildcard include/config/arch/has/scaled/cputime.h) \
    $(wildcard include/config/cpu/freq/times.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/posix/timers.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/ubsan.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/intel/rdt.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/rseq.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/kcov.h) \
    $(wildcard include/config/process/reclaim.h) \
    $(wildcard include/config/uprobes.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/arch/task/struct/on/stack.h) \
    $(wildcard include/config/debug/rseq.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sched.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ipc.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uidgid.h \
    $(wildcard include/config/multiuser.h) \
    $(wildcard include/config/user/ns.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/highuid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rhashtable-types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/osq_lock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/oppocfs/oppo_cfs_mutex.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ipc.h \
  arch/arm64/include/generated/uapi/asm/ipcbuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/ipcbuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/refcount.h \
    $(wildcard include/config/refcount/full.h) \
  arch/arm64/include/generated/uapi/asm/sembuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/sembuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/shm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/page.h \
    $(wildcard include/config/have/arch/pfn/valid.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/personality.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/personality.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/pgtable-types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/pgtable-nopud.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/pgtable-nop4d-hack.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/5level-fixup.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/getorder.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/shm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/hugetlb_encode.h \
  arch/arm64/include/generated/uapi/asm/shmbuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/shmbuf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/shmparam.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/shmparam.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kcov.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/kcov.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/time/low/res.h) \
    $(wildcard include/config/timerfd.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rbtree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timerqueue.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
    $(wildcard include/config/checkpoint/restore.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/seccomp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/seccomp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/unistd.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/unistd.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/unistd.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/unistd.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/seccomp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/unistd.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/nodemask.h \
    $(wildcard include/config/highmem.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/resource.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/resource.h \
  arch/arm64/include/generated/uapi/asm/resource.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/resource.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/resource.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/latencytop.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/prio.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/signal_types.h \
    $(wildcard include/config/old/sigaction.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/signal.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/signal.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/signal.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/signal.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/signal-defs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/siginfo.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/siginfo.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mm_types_task.h \
    $(wildcard include/config/arch/want/batched/unmap/tlb/flush.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/rseq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/task_stack.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/magic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/socket.h \
  arch/arm64/include/generated/uapi/asm/socket.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/socket.h \
  arch/arm64/include/generated/uapi/asm/sockios.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/sockios.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sockios.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uio.h \
    $(wildcard include/config/arch/has/uaccess/mcsafe.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/uio.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/socket.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/libc-compat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/hdlc/ioctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/fs.h \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/cgroup/writeback.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/encryption.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fs/dax.h) \
    $(wildcard include/config/mandatory/file/locking.h) \
    $(wildcard include/config/migration.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/wait_bit.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/wait.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/wait.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/signal.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kdev_t.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/kdev_t.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dcache.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rculist_bl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/list_bl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bit_spinlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  include/generated/bounds.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stringhash.h \
    $(wildcard include/config/dcache/word/access.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/hash.h \
    $(wildcard include/config/have/arch/hash.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/path.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/list_lru.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/shrinker.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/radix-tree.h \
    $(wildcard include/config/radix/tree/multiorder.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/xarray.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
    $(wildcard include/config/rwsem/prio/aware.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/err.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/oppocfs/oppo_cfs_rwsem.h \
  arch/arm64/include/generated/asm/rwsem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/rwsem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mm_types.h \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/userfaultfd.h) \
    $(wildcard include/config/speculative/page/fault.h) \
    $(wildcard include/config/have/arch/compat/mmap/bases.h) \
    $(wildcard include/config/membarrier.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/virtual/reserve/memory.h) \
    $(wildcard include/config/hmm.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/auxvec.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/auxvec.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/auxvec.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/completion.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uprobes.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/uprobes.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/debug-monitors.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/esr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/probes.h \
    $(wildcard include/config/kprobes.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page-flags-layout.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/sparsemem.h \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/hotplug/size/bits.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/mmu.h \
    $(wildcard include/config/unmap/kernel/at/el0.h) \
    $(wildcard include/config/harden/branch/predictor.h) \
    $(wildcard include/config/harden/el2/vectors.h) \
    $(wildcard include/config/memory/hotremove.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/capability.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/capability.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/semaphore.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/fcntl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/fcntl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/fcntl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/fcntl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/fiemap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/migrate_mode.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu-rwsem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcuwait.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcu_sync.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/delayed_call.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uuid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/uuid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/errseq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ioprio.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/rt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/iocontext.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/fs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/limits.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ioctl.h \
  arch/arm64/include/generated/uapi/asm/ioctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/ioctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/ioctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu_counter.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/zone/device.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/cma.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/zsmalloc.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/memory_hotplug.h \
    $(wildcard include/config/arch/has/add/pages.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/notifier.h \
    $(wildcard include/config/tree/srcu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/srcu.h \
    $(wildcard include/config/tiny/srcu.h) \
    $(wildcard include/config/srcu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcu_segcblist.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/srcutree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcu_node_tree.h \
    $(wildcard include/config/rcu/fanout.h) \
    $(wildcard include/config/rcu/fanout/leaf.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/sched/smt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/arch_topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/dqblk_xfs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dqblk_v1.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dqblk_v2.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dqblk_qtree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/projid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/quota.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/nfs_fs_i.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/aio_abi.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uaccess.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/uaccess.h \
    $(wildcard include/config/arm64/pan.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/kernel-pgtable.h \
    $(wildcard include/config/randomize/base.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/pgtable.h \
    $(wildcard include/config/arm64/strict/break/before/make.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/proc-fns.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/pgtable-prot.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/fixmap.h \
    $(wildcard include/config/acpi/apei/ghes.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/boot.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/fixmap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/pgtable.h \
    $(wildcard include/config/have/arch/transparent/hugepage/pud.h) \
    $(wildcard include/config/have/arch/soft/dirty.h) \
    $(wildcard include/config/arch/enable/thp/migration.h) \
    $(wildcard include/config/have/arch/huge/vmap.h) \
    $(wildcard include/config/x86/espfix64.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/extable.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/syscall_wrapper.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ethtool.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/if_ether.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/rx/busy/poll.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
    $(wildcard include/config/netfilter/xt/target/trace.h) \
    $(wildcard include/config/nf/tables.h) \
    $(wildcard include/config/ip/vs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/net.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/random.h \
    $(wildcard include/config/gcc/plugin/latent/entropy.h) \
    $(wildcard include/config/arch/random.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/once.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/random.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/irqnr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/irqnr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/net.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/textsearch.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/slab.h \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/have/hardened/usercopy/allocator.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/overflow.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kasan.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/checksum.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/checksum.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/checksum.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dma-mapping.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/arch/has/dma/set/coherent/mask.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/generic/msi/irq/domain.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/generic/msi/irq.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ioport.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sysfs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/idr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kobject_ns.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kref.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/klist.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ratelimit.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/device.h \
    $(wildcard include/config/iommu/api.h) \
    $(wildcard include/config/xen.h) \
    $(wildcard include/config/arm64/dma/use/iommu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pm_wakeup.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/dma-direction.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
    $(wildcard include/config/debug/sg.h) \
    $(wildcard include/config/sgl/alloc.h) \
    $(wildcard include/config/arch/has/sg/chain.h) \
    $(wildcard include/config/sg/pool.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mm.h \
    $(wildcard include/config/have/arch/mmap/rnd/bits.h) \
    $(wildcard include/config/have/arch/mmap/rnd/compat/bits.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/arch/uses/high/vma/flags.h) \
    $(wildcard include/config/arch/has/pkeys.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/parisc.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/sparc64.h) \
    $(wildcard include/config/x86/intel/mpx.h) \
    $(wildcard include/config/dev/pagemap/ops.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/have/memblock.h) \
    $(wildcard include/config/debug/vm/rb.h) \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/range.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu-refcount.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page_ext.h \
    $(wildcard include/config/idle/page/tracking.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stackdepot.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page_ref.h \
    $(wildcard include/config/debug/page/ref.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page-flags.h \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/thp/swap.h) \
    $(wildcard include/config/ksm.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/tracepoint-defs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/static_key.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/memremap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/huge_mm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/coredump.h \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
    $(wildcard include/config/debug/tlbflush.h) \
    $(wildcard include/config/debug/vm/vmacache.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vm_event_item.h \
    $(wildcard include/config/memory/balloon.h) \
    $(wildcard include/config/balloon/compaction.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/io.h \
  arch/arm64/include/generated/asm/early_ioremap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/early_ioremap.h \
    $(wildcard include/config/generic/early/ioremap.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/msm_rtb.h \
    $(wildcard include/config/qcom/rtb.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/xen/xen.h \
    $(wildcard include/config/xen/pvh.h) \
    $(wildcard include/config/xen/dom0.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/io.h \
    $(wildcard include/config/generic/iomap.h) \
    $(wildcard include/config/has/ioport/map.h) \
    $(wildcard include/config/virt/to/bus.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/pci_iomap.h \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/logic_pio.h \
    $(wildcard include/config/indirect/pio.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/fwnode.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vmalloc.h \
    $(wildcard include/config/enable/vmalloc/saving.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mem_encrypt.h \
    $(wildcard include/config/arch/has/mem/encrypt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/dma-mapping.h \
    $(wildcard include/config/iommu/dma.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/xen/hypervisor.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/xen/arm/hypervisor.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netdev_features.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/clock.h \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/flow_dissector.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/in6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/in6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_ether.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/splice.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pipe_fs_i.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_packet.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/flow.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/imq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/net_namespace.h \
    $(wildcard include/config/ieee802154/6lowpan.h) \
    $(wildcard include/config/ip/sctp.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/netfilter/netlink/acct.h) \
    $(wildcard include/config/nf/ct/netlink/timeout.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/mpls.h) \
    $(wildcard include/config/can.h) \
    $(wildcard include/config/net/ns.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sysctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sysctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/core.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/snmp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/snmp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/u64_stats_sync.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/unix.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/packet.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/ip/route/classid.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
    $(wildcard include/config/ip/route/multipath.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/inet_frag.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/siphash.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/dst_ops.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/ieee802154_6lowpan.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/sctp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/dccp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/netfilter.h \
    $(wildcard include/config/netfilter/family/arp.h) \
    $(wildcard include/config/netfilter/family/bridge.h) \
    $(wildcard include/config/nf/defrag/ipv4.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netfilter_defs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netfilter.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/in.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/in.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/conntrack.h \
    $(wildcard include/config/nf/ct/proto/dccp.h) \
    $(wildcard include/config/nf/ct/proto/sctp.h) \
    $(wildcard include/config/nf/conntrack/events.h) \
    $(wildcard include/config/nf/conntrack/labels.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/list_nulls.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netfilter/nf_conntrack_tcp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netfilter/nf_conntrack_tcp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netfilter/nf_conntrack_dccp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netfilter/nf_conntrack_tuple_common.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netfilter/nf_conntrack_common.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netfilter/nf_conntrack_common.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netfilter/nf_conntrack_sctp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netfilter/nf_conntrack_sctp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/nftables.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/xfrm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/xfrm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/mpls.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/can.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ns_common.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/seq_file_net.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/seq_file.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/oppo/fg/opt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/key.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/user.h \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/bpf/syscall.h) \
    $(wildcard include/config/net.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netprio_cgroup.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cgroup.h \
    $(wildcard include/config/cgroup/cpuacct.h) \
    $(wildcard include/config/sock/cgroup/data.h) \
    $(wildcard include/config/cgroup/net/classid.h) \
    $(wildcard include/config/cgroup/data.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/cgroupstats.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/taskstats.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/nsproxy.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/user_namespace.h \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/persistent/keyrings.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kernel_stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
    $(wildcard include/config/irq/timings.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/irqreturn.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/hardirq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
    $(wildcard include/config/hwlat/tracer.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vtime.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/context_tracking_state.h \
    $(wildcard include/config/context/tracking.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/hardirq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/irq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/irq.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/kvm_arm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/irq_cpustat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cgroup-defs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bpf-cgroup.h \
    $(wildcard include/config/cgroup/bpf.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/bpf.h \
    $(wildcard include/config/efficient/unaligned/access.h) \
    $(wildcard include/config/bpf/kprobe/override.h) \
    $(wildcard include/config/function/error/injection.h) \
    $(wildcard include/config/bpf/lirc/mode2.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/bpf_common.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/psi_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kthread.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cgroup_subsys.h \
    $(wildcard include/config/sched/tune.h) \
    $(wildcard include/config/cgroup/device.h) \
    $(wildcard include/config/cgroup/freezer.h) \
    $(wildcard include/config/cgroup/perf.h) \
    $(wildcard include/config/cgroup/hugetlb.h) \
    $(wildcard include/config/cgroup/pids.h) \
    $(wildcard include/config/cgroup/rdma.h) \
    $(wildcard include/config/cgroup/debug.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/xdp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/neighbour.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/netlink.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/scm.h \
    $(wildcard include/config/security/network.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/security.h \
    $(wildcard include/config/security/infiniband.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/securityfs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bio.h \
    $(wildcard include/config/pfk.h) \
    $(wildcard include/config/blk/dev/integrity.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/highmem.h \
    $(wildcard include/config/arch/want/kmap/atomic/flush.h) \
    $(wildcard include/config/x86/32.h) \
    $(wildcard include/config/debug/highmem.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/cacheflush.h \
    $(wildcard include/config/kgdb.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kgdb.h \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/serial/kgdb/nmi.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/kgdb.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ptrace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/signal.h \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/jobctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched/task.h \
    $(wildcard include/config/have/copy/thread/tls.h) \
    $(wildcard include/config/have/exit/thread.h) \
    $(wildcard include/config/arch/wants/dynamic/task/struct.h) \
    $(wildcard include/config/have/arch/thread/struct/whitelist.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pid_namespace.h \
    $(wildcard include/config/pid/ns.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ptrace.h \
  arch/arm64/include/generated/asm/kmap_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/kmap_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mempool.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/blk_types.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/dm/default/key.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bvec.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netlink.h \
    $(wildcard include/config/oppo/hans.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/netdevice.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/if_link.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_link.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_bonding.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/pkt_cls.h \
    $(wildcard include/config/net/cls/ind.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/pkt_sched.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/hashtable.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ip.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ip.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ipv6.h \
    $(wildcard include/config/ipv6/router/pref.h) \
    $(wildcard include/config/ipv6/route/info.h) \
    $(wildcard include/config/ipv6/optimistic/dad.h) \
    $(wildcard include/config/ipv6/seg6/hmac.h) \
    $(wildcard include/config/ipv6/mip6.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/ipv6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/icmpv6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/icmpv6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/tcp.h \
    $(wildcard include/config/smc.h) \
    $(wildcard include/config/bpf.h) \
    $(wildcard include/config/tcp/md5sig.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/win_minmax.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/sock.h \
    $(wildcard include/config/sock/validate/xmit.h) \
    $(wildcard include/config/inet.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page_counter.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/memcontrol.h \
    $(wildcard include/config/memcg/swap.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vmpressure.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/eventfd.h \
    $(wildcard include/config/eventfd.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/writeback.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/flex_proportions.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/backing-dev-defs.h \
    $(wildcard include/config/debug/fs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/filter.h \
    $(wildcard include/config/bpf/jit.h) \
    $(wildcard include/config/bpf/jit/always/on.h) \
    $(wildcard include/config/have/ebpf/jit.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cryptohash.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/set_memory.h \
    $(wildcard include/config/arch/has/set/memory.h) \
  arch/arm64/include/generated/asm/set_memory.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/set_memory.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kallsyms.h \
    $(wildcard include/config/kallsyms/all.h) \
    $(wildcard include/config/kallsyms.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/module.h \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/retpoline.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kmod.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/umh.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/elf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/elf.h \
  arch/arm64/include/generated/asm/user.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/user.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/elf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/elf-em.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/moduleparam.h \
    $(wildcard include/config/ppc64.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rbtree_latch.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/error-injection.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/error-injection.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cfi.h \
    $(wildcard include/config/cfi/clang/shadow.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/module.h \
    $(wildcard include/config/arm64/module/plts.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/if_vlan.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/etherdevice.h \
  arch/arm64/include/generated/asm/unaligned.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/unaligned.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/unaligned/access_ok.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/unaligned/generic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rtnetlink.h \
    $(wildcard include/config/net/ingress.h) \
    $(wildcard include/config/net/egress.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/rtnetlink.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_addr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_vlan.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/sch_generic.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/gen_stats.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/gen_stats.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/rtnetlink.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netlink.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/filter.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rculist_nulls.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/poll.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/poll.h \
  arch/arm64/include/generated/uapi/asm/poll.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/poll.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/eventpoll.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/dst.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/neighbour.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/tcp_states.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/net_tstamp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/smc.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/l3mdev.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/fib_rules.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/fib_rules.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/fib_notifier.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/inet_connection_sock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/inet_sock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/jhash.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/unaligned/packed_struct.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/request_sock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/netns/hash.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/inet_timewait_sock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/timewait_sock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/tcp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/udp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/udp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/ip6_checksum.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/ip.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/route.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/inetpeer.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/ipv6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/if_inet6.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/ndisc.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/if_arp.h \
    $(wildcard include/config/firewire/net.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/if_arp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/ip_fib.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/in_route.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/route.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/udp.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../drivers/net/ethernet/qualcomm/rmnet/rmnet_map.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../drivers/net/ethernet/qualcomm/rmnet/rmnet_config.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/net/gro_cells.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_opt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_core.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../drivers/net/ethernet/qualcomm/rmnet/rmnet_descriptor.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_config.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../drivers/net/ethernet/qualcomm/rmnet/rmnet_handlers.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../drivers/net/ethernet/qualcomm/rmnet/rmnet_trace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/tracepoint.h \
    $(wildcard include/config/have/syscall/tracepoints.h) \
    $(wildcard include/config/tracepoint.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/trace/define_trace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/../include/soc/qcom/qmi_rmnet.h \
    $(wildcard include/config/qcom/qmi/rmnet.h) \
    $(wildcard include/config/qcom/qmi/dfc.h) \
    $(wildcard include/config/qcom/qmi/power/collapse.h) \

/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o: $(deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o)

$(deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf_udp_opt.o):
