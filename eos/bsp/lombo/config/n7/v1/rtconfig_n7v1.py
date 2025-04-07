import os
from rtconfig import *
import prof_exclude_f

#========================================================================================
# BUILD parament. you should choose one according to need
#   BUILD = 'debug': build sdk with debug level. eg: -O0..
#   BUILD = 'release_dev': build sdk with release level(eg: -O2), for development
#   BUILD = 'release_mp': build sdk with release level(eg: -O2), only for mass production
#      Some extra operation will be added for release_mp, which is unnecessary or
#      improper in release_dev.
#
# gcc options. you could modify them according to debug need
# (1). OPT_LVL: code optimize level.
#      0: no optimization
#      2: normal optimization
#          this value is suggested to 2 for sdk release version, and 0 for debug version,
#        when set to 0, the code size will be very big.
# (2). DUMP_EXCP_STACK: dump the calling stack when exception.
#      0: not dump the exception stack
#      1: dump the exception stack
#          this value is suggested to 0 for sdk release version, and 1 for debug version,
# (3). FUNC_PROFILE: trace all functions entry/exit point, for profile.
#      0: no profile
#      1: have profile
#          if it is 1, the __cyg_profile_func_enter/exit was called when enter/exit
#        all functions. this value is suggested to 0 for sdk release version, and 1 for
#        debug version.
# (4). PROFILE_OMIT_FUNC and PROFILE_OMIT_FILE: used when FUNC_PROFILE is 1, to omit
#        some type of functions.
#          1) if we want to skip the functions in perticular files, use:
#            "-finstrument-functions-exclude-file-list=/name1,name2/name3"
#       this skip all functions in files that contain substring "/name1" or "name2/name3"
#          2) if we want to skip the functions with certain substring, use:
#            "-finstrument-functions-exclude-function-list=func1,func2,..."
#       this skip all functions that contain substring "func1" or "func2"..., for example
#       abcfunc1def, func2foo...
#          3) if we want to skip a certain function, use:
#            "__attribute__((no_instrument_function))" in the function defination line
#========================================================================================

PROFILE_EXCLUDE_DIRS = ',' + prof_exclude_f.exclude_dirs()
# print('profile exclude directories:\n' + PROFILE_EXCLUDE_DIRS)

#BUILD = 'debug'
#BUILD = 'release_dev'
BUILD = 'release_mp'
if BUILD == 'debug':
    OPT_LVL = '0'
    DUMP_EXCP_STACK = '1'
    FUNC_PROFILE = '0'
    PROFILE_OMIT_FUNC = 'rt_,vector,cache,mmu,irq,interrupt,printf,malloc,free,csp'
    PROFILE_OMIT_FILE = PROFILE_OMIT_FUNC + ',libcpu,src,components,_dump' + PROFILE_EXCLUDE_DIRS
    KENERL_DEF = " -D__EOS__DEBUG__ "
elif BUILD == 'release_dev':
    OPT_LVL = '2'
    DUMP_EXCP_STACK = '1'
    FUNC_PROFILE = '0'
    KENERL_DEF = " -D__EOS__RELEASE__ "
elif BUILD == 'release_mp':
    OPT_LVL = '2'
    DUMP_EXCP_STACK = '1'
    FUNC_PROFILE = '0'
    KENERL_DEF = " -D__EOS__RELEASE__ -D__EOS__RELEASE__MP__ "
else:
    print('BUILD para \"' + BUILD + '\" err, please re-select!')

# toolchains options
ARCH='arm'
CPU='cortex-a'
CROSS_TOOL='gcc'

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

# only support GNU GCC compiler.
PLATFORM  = 'gcc'
EXEC_PATH = os.getenv('TOP_DIR') + '/tools/toolchain/gcc-arm-none-eabi-7-2017-q4-major/bin'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'arm-none-eabi-'
    CC = PREFIX + 'gcc'
    CXX = PREFIX + 'g++'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'
    STRIP = PREFIX + 'strip'

    #DEVICE = ' -march=armv7-a -mtune=cortex-a7 -mfpu=vfpv4-d16 -ftree-vectorize -ffast-math -mfloat-abi=softfp'
    DEVICE = ' -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -ftree-vectorize -ffast-math -mfloat-abi=hard'
    DEVICE += ' -ffunction-sections -fdata-sections '
    DEVICE += ' -Wno-unused-but-set-variable '

    DEVICE += KENERL_DEF
    DEVICE += ' -D_POSIX_C_SOURCE=2 -D__POSIX_VISIBLE=199209 '
    if FUNC_PROFILE == '1':
        DEVICE += ' -finstrument-functions -DFUNC_PROFILE '
        DEVICE += ' -finstrument-functions-exclude-function-list=' + PROFILE_OMIT_FUNC
        DEVICE += ' -finstrument-functions-exclude-file-list=' + PROFILE_OMIT_FILE
    if DUMP_EXCP_STACK == '1':
        DEVICE += ' -fno-omit-frame-pointer -DDUMP_EXCP_STACK '

    CFLAGS = DEVICE + ' -Wall'
    CFLAGS += ' -O' + OPT_LVL
    AFLAGS = ' -c' + DEVICE + ' -x assembler-with-cpp -D__ASSEMBLY__'
    LINK_SCRIPT = BSP_CFG_SOC + '/lombo_' + LOMBO_CPU + LOMBO_SOC + '.ld'
    LFLAGS = DEVICE + ' -Wl,--gc-sections,-u,system_vectors -T %s' % LINK_SCRIPT
    LFLAGS += ' -Wl,--whole-archive ./libraries/release/*.a -Wl,--no-whole-archive '

    CPATH = ''
    LPATH = ''

    # generate debug info in all cases
    AFLAGS += ' -gdwarf-2'
    CFLAGS += ' -g -gdwarf-2'

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' +\
                  SIZE + ' $TARGET \n'

    # extra include path
    EX_INC = ' -include "rtconfig.h" '
    EX_INC += ' -include "eos.h" '
    EX_INC += ' -I"' + TOP_DIR + '"/include '
    EX_INC += ' -I"' + KERN_DIR + '"/include '
    EX_INC += ' -I"' + BSP_DIR + '"/include'
    EX_INC += ' -I"' + BSP_DIR + '"/include/platform'
    EX_INC += ' -I"' + BSP_DIR + '"/include/platform/' + LOMBO_CPU

    CFLAGS += EX_INC
    AFLAGS += EX_INC

    POST_ACTION = OBJCPY + ' -O binary $TARGET rtthread.bin\n' +\
			SIZE + ' $TARGET \n'

    # dynamic module flags
    CXXFLAGS = CFLAGS + ' -Woverloaded-virtual -fno-exceptions -fno-rtti'
    M_CFLAGS = CFLAGS + ' -mlong-calls -fPIC '
    M_CXXFLAGS = CXXFLAGS + ' -mlong-calls -fPIC'
    M_LFLAGS = CXXFLAGS + ' -Wl,--gc-sections,-z,max-page-size=0x4' +\
			' -shared -fPIC -nostartfiles -nostdlib -static-libgcc'
    M_POST_ACTION = STRIP + ' --strip-unneeded -R .hash $TARGET\n' + SIZE + ' $TARGET \n'
    # M_BIN_PATH = r'~/'
