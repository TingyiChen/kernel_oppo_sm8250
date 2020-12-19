cmd_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o := python /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/scripts/gcc-wrapper.py /work/oppo_80300754/find_x2_full/SM8250_Q/android/vendor/qcom/proprietary/llvm-arm-toolchain-ship/8.0/bin/clang -Wp,-MD,/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/.rmnet_perf.mod.o.d  -nostdinc -isystem /work/oppo_80300754/find_x2_full/SM8250_Q/android/vendor/qcom/proprietary/llvm-arm-toolchain-ship/8.0/lib/clang/8.0.12/include -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include -I./arch/arm64/include/generated  -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include -I./include -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi -I./arch/arm64/include/generated/uapi -I/work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi -I./include/generated/uapi -include /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kconfig.h -include /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler_types.h  -I/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf -I/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf -D__KERNEL__ -DVENDOR_EDIT -mlittle-endian -Qunused-arguments -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -DVENDOR_EDIT --target=aarch64-linux-gnu --prefix=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/ --gcc-toolchain=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 -no-integrated-as -Werror=unknown-warning-option -fno-PIE -mno-implicit-float -DCONFIG_AS_LSE=1 -fno-asynchronous-unwind-tables -fno-delete-null-pointer-checks -Wno-address-of-packed-member -O2 -Wframe-larger-than=2048 -fstack-protector-strong --target=aarch64-linux-gnu --gcc-toolchain=/work/oppo_80300754/find_x2_full/SM8250_Q/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9 -meabi gnu -Wno-format-invalid-specifier -Wno-gnu -Wno-duplicate-decl-specifier -Wno-asm-operand-widths -Wno-initializer-overrides -fno-builtin -Wno-undefined-optimized -Wno-tautological-constant-out-of-range-compare -mllvm -disable-struct-const-merge -Wno-tautological-compare -mno-global-merge -Wno-unused-const-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wvla -g -pg -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fno-merge-all-constants -fmerge-constants -fno-stack-check -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time -Werror=incompatible-pointer-types -Wno-initializer-overrides -Wno-unused-value -Wno-format -Wno-sign-compare -Wno-format-zero-length -Wno-uninitialized  -DKBUILD_BASENAME='"rmnet_perf.mod"' -DKBUILD_MODNAME='"rmnet_perf"' -DMODULE  -DVENDOR_EDIT -c -o /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.c

source_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o := /work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.c

deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o := \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/retpoline.h) \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/build-salt.h \
    $(wildcard include/config/build/salt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/elfnote.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/elf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/elf.h \
    $(wildcard include/config/compat.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/hwcap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/hwcap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/ptrace.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/ptrace.h \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/sigcontext.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/bug/on/data/corruption.h) \
    $(wildcard include/config/panic/on/data/corruption.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/bug.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stringify.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/asm-bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/brk-imm.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/smp.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
    $(wildcard include/config/kasan.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/barrier.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/barrier.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kasan-checks.h \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/export.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
    $(wildcard include/config/unused/symbols.h) \
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
    $(wildcard include/config/dynamic/debug.h) \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/ptrace.h \
  arch/arm64/include/generated/asm/user.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/user.h \
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
    $(wildcard include/config/arm64/sw/ttbr0/pan.h) \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/memory.h \
    $(wildcard include/config/arm64/va/bits.h) \
    $(wildcard include/config/kasan/extra.h) \
    $(wildcard include/config/vmap/stack.h) \
    $(wildcard include/config/debug/align/rodata.h) \
    $(wildcard include/config/blk/dev/initrd.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/const.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/const.h \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/fpsimd.h \
  arch/arm64/include/generated/uapi/asm/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/errno.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/errno-base.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/elf.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/elf-em.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/module.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/function/error/injection.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
    $(wildcard include/config/page/poisoning/zero.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/uapi/asm/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/asm-generic/stat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/compat_time.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/time64.h \
    $(wildcard include/config/64bit/time.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  arch/arm64/include/generated/asm/div64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/div64.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/time.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/compat.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sched.h \
    $(wildcard include/config/oppo/healthinfo.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/sched/info.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/sched/walt.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/process/reclaim/enhance.h) \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/preempt/notifiers.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tasks/rcu.h) \
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
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/posix/timers.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/ubsan.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/intel/rdt.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/numa.h) \
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
    $(wildcard include/config/security.h) \
    $(wildcard include/config/arch/task/struct/on/stack.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/rseq.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sched.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/current.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rculist.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcupdate.h \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/arch/weak/release/acquire.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/irqflags.h \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/irqflags.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/trace/preempt/toggle.h) \
  arch/arm64/include/generated/asm/preempt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/preempt.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/thread_info.h \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/restart_block.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/thread_info.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/stack_pointer.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bottom_half.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/lockdep.h \
    $(wildcard include/config/lock/stat.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/stacktrace.h \
    $(wildcard include/config/stacktrace.h) \
    $(wildcard include/config/user/stacktrace/support.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/bitmap.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcutree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ipc.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/spinlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/spinlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qspinlock_types.h \
    $(wildcard include/config/paravirt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qrwlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwlock_types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/spinlock.h \
  arch/arm64/include/generated/asm/qrwlock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/qrwlock.h \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/uidgid.h \
    $(wildcard include/config/multiuser.h) \
    $(wildcard include/config/user/ns.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/highuid.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rhashtable-types.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/osq_lock.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/oppocfs/oppo_cfs_mutex.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/timer.h \
    $(wildcard include/config/debug/objects/timers.h) \
    $(wildcard include/config/no/hz/common.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/ktime.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/seqlock.h \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
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
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kmod.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/umh.h \
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
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/wait.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/wait.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/signal.h \
    $(wildcard include/config/proc/fs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/page-flags-layout.h \
  include/generated/bounds.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/sparsemem.h \
    $(wildcard include/config/hotplug/size/bits.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/memory_hotplug.h \
    $(wildcard include/config/arch/has/add/pages.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/notifier.h \
    $(wildcard include/config/tree/srcu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
    $(wildcard include/config/rwsem/prio/aware.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/err.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/oppocfs/oppo_cfs_rwsem.h \
  arch/arm64/include/generated/asm/rwsem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/rwsem.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/srcu.h \
    $(wildcard include/config/tiny/srcu.h) \
    $(wildcard include/config/srcu.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcu_segcblist.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/srcutree.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rcu_node_tree.h \
    $(wildcard include/config/rcu/fanout.h) \
    $(wildcard include/config/rcu/fanout/leaf.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/completion.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/sched/smt.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/arch_topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/topology.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/uapi/linux/sysctl.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/sysfs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/idr.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/radix-tree.h \
    $(wildcard include/config/radix/tree/multiorder.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kobject_ns.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/kref.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/rbtree_latch.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/error-injection.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/error-injection.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/cfi.h \
    $(wildcard include/config/cfi/clang/shadow.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/tracepoint-defs.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/static_key.h \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/arch/arm64/include/asm/module.h \
    $(wildcard include/config/arm64/module/plts.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
    $(wildcard include/config/randomize/base.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  /work/oppo_80300754/find_x2/SM8250_Q/android/kernel/msm-4.19/include/linux/vermagic.h \
  include/generated/utsrelease.h \

/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o: $(deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o)

$(deps_/work/oppo_80300754/find_x2/SM8250_Q/android/vendor/qcom/opensource/data-kernel/drivers/rmnet/perf/rmnet_perf.mod.o):
