#
# the compressed image build script
#
GCOV_PROFILE := n
CROSS_COMPILE=../../../../../tools/toolchain/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-
quiet=@

# the building output dir
outdir=../../../../../out/build/bsp/system/compressed
bspdir=../..

# the target file
prefix=rtthread-z
target=$(bspdir)/$(prefix).bin
target_axf=$(outdir)/$(prefix).axf
target_map=$(outdir)/$(prefix).map
target_lst=$(outdir)/$(prefix).lst
origin_bin=$(bspdir)/rtthread.bin
libs=libcomp-${LOMBO_CPU}${LOMBO_SOC}-${LOMBO_BOARD}.a

$(target):

# read LOMBO_DRAM_PBASE's value from memory.h
LOMBO_DRAM_PBASE=`sed -n '/^\#define LOMBO_DRAM_PBASE/p' ../../rtconfig.h | awk '{print $$3}'`

CFLAGS := -g -c -nostdlib -march=armv7-a -marm -fno-unwind-tables -fno-asynchronous-unwind-tables -mlittle-endian -Os -mno-unaligned-access
CFLAGS += -I "../../include" -I "../../include/platform" -include "../../rtconfig.h" -include "../../include/eos.h" -I "./include" -I "./"
CFLAGS += -Wno-builtin-declaration-mismatch
CFLAGS += -mfloat-abi=hard -mfpu=vfp
CFLAGS += -fpic -mno-single-pic-base -fno-builtin

AFLAGS := $(CFLAGS)
AFLAGS += -D__ASSEMBLY__
AFLAGS += -fpic -mno-single-pic-base -fno-builtin

LD_FLAGS = -static --defsym=LOMBO_DRAM_PBASE=$(LOMBO_DRAM_PBASE)

#
# LZO do not include size in file... so we have to fake that;
# append the size as a 32-bit littleendian number as gzip does.
#
size_append = printf $(shell						\
dec_size=0;								\
for F in $1; do								\
	fsize=$$(stat -c "%s" $$F);					\
	dec_size=$$(expr $$dec_size + $$fsize);				\
done;									\
printf "%08x\n" $$dec_size |						\
	sed 's/\(..\)/\1 /g' | {					\
		read ch0 ch1 ch2 ch3;					\
		for ch in $$ch3 $$ch2 $$ch1 $$ch0; do			\
			printf '%s%03o' '\\' $$((0x$$ch)); 		\
		done;							\
	}								\
)

$(outdir)/%.o: %.c
	@echo "CC    $@"
	$(quiet) $(CROSS_COMPILE)gcc $(CFLAGS) -o $@ $<

$(outdir)/%.o: lib/%.c
	@echo "CC    $@"
	$(quiet) $(CROSS_COMPILE)gcc $(CFLAGS) -o $@ $<

$(outdir)/%.o: %.S
	@echo "CC    $@"
	$(quiet) $(CROSS_COMPILE)gcc $(AFLAGS) -o $@ $<

$(outdir)/%.o: lib/%.S
	@echo "CC    $@"
	$(quiet) $(CROSS_COMPILE)gcc $(AFLAGS) -o $@ $<

$(outdir)/piggy.o: piggy.S FORCE
	@echo "CC    $@"
	$(quiet) $(CROSS_COMPILE)gcc $(AFLAGS) -o $@ $<

piggy.S: piggy.gz piggy.lzo FORCE

piggy.gz: FORCE
	@echo "gen   $@"
	$(quiet) (cat $(origin_bin) | gzip -n -f -9) > $@

piggy.lzo: FORCE
	@echo "gen   $@"
	$(quiet) (cat $(origin_bin) | lzop -9 && $(call size_append, $(origin_bin))) > $@

$(target_axf): $(libs) $(outdir)/piggy.o load.scf
	$(CROSS_COMPILE)ld -T load.scf $(LD_FLAGS) -EL --whole-archive -L. -l:$(libs) \
		$(outdir)/piggy.o -o $(target_axf) -Map $(target_map)

$(target): $(target_axf)
	$(CROSS_COMPILE)objcopy -O binary $< $@

clean-files = $(target) $(target_axf) piggy.gz piggy.lzo
clean: FORCE
	$(quiet) rm -rf $(clean-files) $(outdir)
	$(quiet) mkdir -p $(outdir)

#
# add lib/Makefile if exist
#
ifeq (lib/Makefile, $(wildcard lib/Makefile))
include lib/Makefile
endif

FORCE:
.PHONY: FORCE
