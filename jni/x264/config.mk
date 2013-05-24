SRCPATH=$(LOCAL_PATH)
includedir=${prefix}/include
X264_CFLAGS :=-Wshadow -O3 -ffast-math   -Wall -I. -I$(SRCPATH) -std=c99 -fomit-frame-pointer -fno-tree-vectorize \
              -fno-zero-initialized-in-bss 
X264_LDFLAGS:= -Wl,--large-address-aware -lpthread
SRCS := common/mc.c common/predict.c common/pixel.c common/macroblock.c \
       common/frame.c common/dct.c common/cpu.c common/cabac.c \
       common/common.c common/osdep.c common/rectangle.c \
       common/set.c common/quant.c common/deblock.c common/vlc.c \
       common/mvpred.c common/bitstream.c \
       encoder/analyse.c encoder/me.c encoder/ratecontrol.c \
       encoder/set.c encoder/macroblock.c encoder/cabac.c \
       encoder/cavlc.c encoder/encoder.c encoder/lookahead.c

SRCCLI := x264.c input/input.c input/timecode.c input/raw.c input/y4m.c \
         output/raw.c output/matroska.c output/matroska_ebml.c \
         output/flv.c output/flv_bytestream.c filters/filters.c \
         filters/video/video.c filters/video/source.c filters/video/internal.c \
         filters/video/resize.c filters/video/cache.c filters/video/fix_vfr_pts.c \
         filters/video/select_every.c filters/video/crop.c filters/video/depth.c
ASMSRC +:= common/arm/cpu-a.S common/arm/pixel-a.S common/arm/mc-a.S \
          common/arm/dct-a.S common/arm/quant-a.S common/arm/deblock-a.S \
          common/arm/predict-a.S
SRCS   +:= common/arm/mc-c.c common/arm/predict-c.c
CONFIG := $(shell cat config.h)

# GPL-only files
ifneq ($(findstring HAVE_GPL 1, $(CONFIG)),)
SRCCLI +=
endif

# Optional module sources
ifneq ($(findstring HAVE_AVS 1, $(CONFIG)),)
SRCCLI += input/avs.c
endif

ifneq ($(findstring HAVE_THREAD 1, $(CONFIG)),)
SRCCLI += input/thread.c
SRCS   += common/threadpool.c
endif

ifneq ($(findstring HAVE_WIN32THREAD 1, $(CONFIG)),)
SRCS += common/win32thread.c
endif

ifneq ($(findstring HAVE_LAVF 1, $(CONFIG)),)
SRCCLI += input/lavf.c
endif

ifneq ($(findstring HAVE_FFMS 1, $(CONFIG)),)
SRCCLI += input/ffms.c
endif

ifneq ($(findstring HAVE_GPAC 1, $(CONFIG)),)
SRCCLI += output/mp4.c
endif

# Visualization sources
ifneq ($(findstring HAVE_VISUALIZE 1, $(CONFIG)),)
SRCS   += common/visualize.c common/display-x11.c
endif

# MMX/SSE optims
ifneq ($(AS),)
X86SRC0 = const-a.asm cabac-a.asm dct-a.asm deblock-a.asm mc-a.asm \
          mc-a2.asm pixel-a.asm predict-a.asm quant-a.asm \
          cpu-a.asm dct-32.asm bitstream-a.asm
ifneq ($(findstring HIGH_BIT_DEPTH, $(CONFIG)),)
X86SRC0 += sad16-a.asm
else
X86SRC0 += sad-a.asm
endif
X86SRC = $(X86SRC0:%=common/x86/%)

ifeq ($(ARCH),X86)
ARCH_X86 = yes
ASMSRC   = $(X86SRC) common/x86/pixel-32.asm
ASFLAGS += -DARCH_X86_64=0
endif

ifeq ($(ARCH),X86_64)
ARCH_X86 = yes
ASMSRC   = $(X86SRC:-32.asm=-64.asm) common/x86/trellis-64.asm
ASFLAGS += -DARCH_X86_64=1
endif

ifdef ARCH_X86
ASFLAGS += -I$(SRCPATH)/common/x86/
SRCS   += common/x86/mc-c.c common/x86/predict-c.c
OBJASM  = $(ASMSRC:%.asm=%.o)
$(OBJASM): common/x86/x86inc.asm common/x86/x86util.asm
OBJCHK += tools/checkasm-a.o
endif
endif

# AltiVec optims
ifeq ($(ARCH),PPC)
ifneq ($(AS),)
SRCS += common/ppc/mc.c common/ppc/pixel.c common/ppc/dct.c \
        common/ppc/quant.c common/ppc/deblock.c \
        common/ppc/predict.c
endif
endif

# NEON optims
ifeq ($(ARCH),ARM)
ifneq ($(AS),)
ASMSRC += common/arm/cpu-a.S common/arm/pixel-a.S common/arm/mc-a.S \
          common/arm/dct-a.S common/arm/quant-a.S common/arm/deblock-a.S \
          common/arm/predict-a.S
SRCS   += common/arm/mc-c.c common/arm/predict-c.c
OBJASM  = $(ASMSRC:%.S=%.o)
endif
endif

# VIS optims
ifeq ($(ARCH),UltraSPARC)
ifeq ($(findstring HIGH_BIT_DEPTH, $(CONFIG)),)
ASMSRC += common/sparc/pixel.asm
OBJASM  = $(ASMSRC:%.asm=%.o)
endif
endif

ifneq ($(HAVE_GETOPT_LONG),1)
SRCCLI += extras/getopt.c
endif

ifeq ($(SYS),WINDOWS)
OBJCLI += $(if $(RC), x264res.o)
ifneq ($(SONAME),)
SRCSO  += x264dll.c
OBJSO  += $(if $(RC), x264res.dll.o)
endif
endif