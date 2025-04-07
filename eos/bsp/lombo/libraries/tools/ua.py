import os
import sys
from SCons.Script import *

Rtt_Root = ''
BSP_Root = ''
Env = None

def BuildEnv(BSP_ROOT, RTT_ROOT):
    if BSP_ROOT == None:
        if os.getenv('BSP_ROOT'):
            BSP_ROOT = os.getenv('BSP_ROOT')
        else:
            print 'Please set BSP(board support package) directory!'
            exit(-1)

    if not os.path.exists(BSP_ROOT):
        print 'No BSP(board support package) directory found!'
        exit(-1)

    if RTT_ROOT == None:
        # get RTT_ROOT from BSP_ROOT
        sys.path = sys.path + [BSP_ROOT]
        try:
            import rtconfig
            RTT_ROOT = rtconfig.RTT_ROOT
        except Exception as e:
            print 'Import rtconfig.py in BSP(board support package) failed.'
            print e
            exit(-1)

    global Rtt_Root
    global BSP_Root

    Rtt_Root = RTT_ROOT
    BSP_Root = BSP_ROOT

def BuildHostApplication(TARGET, SConscriptFile):
    import platform

    platform_type = platform.system()
    if platform_type == 'Windows' or platform_type.find('MINGW') != -1:
        TARGET = TARGET.replace('.mo', '.exe')

    sys.path = sys.path + [os.path.join(os.getcwd(), 'tools', 'host')]

    from building import PrepareHostModuleBuilding

    HostRtt = os.path.join(os.getcwd(), 'tools', 'host', 'rtthread')
    Env = Environment()

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareHostModuleBuilding(Env)

    objs = SConscript(SConscriptFile)
    objs += SConscript(HostRtt + '/SConscript')

    target = Env.Program(TARGET, objs)
    return

def BuildHostLibrary(TARGET, SConscriptFile):
    import platform

    platform_type = platform.system()
    if platform_type == 'Windows' or platform_type.find('MINGW') != -1:
        TARGET = TARGET.replace('.mo', '.exe')

    sys.path = sys.path + [os.path.join(os.getcwd(), 'tools', 'host')]

    from building import PrepareHostModuleBuilding

    HostRtt = os.path.join(os.getcwd(), 'tools', 'host', 'rtthread')
    Env = Environment()

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareHostModuleBuilding(Env)

    objs = SConscript(SConscriptFile)
    objs += SConscript(HostRtt + '/SConscript')

    target = Env.Program(TARGET, objs)
    return

def BuildApplication(TARGET, SConscriptFile, BSP_ROOT = None, RTT_ROOT = None):
    global Env
    global Rtt_Root
    global BSP_Root

    # add comstr option
    AddOption('--verbose',
                dest='verbose',
                action='store_true',
                default=False,
                help='print verbose information during build')

    # build application in host
    if BSP_ROOT == None and RTT_ROOT == None and not os.getenv('BSP_ROOT'):
        BuildHostApplication(TARGET, SConscriptFile)
        return

    if RTT_ROOT == None and os.getenv('RTT_ROOT'):
        RTT_ROOT = os.getenv('RTT_ROOT')

    # handle BSP_ROOT and RTT_ROOT
    BuildEnv(BSP_ROOT, RTT_ROOT)

    sys.path = sys.path + [os.path.join(Rtt_Root, 'tools'), BSP_Root]

    # get configuration from BSP
    import rtconfig
    from rtua import GetCPPPATH
    from rtua import GetCPPDEFINES
    from building import PrepareModuleBuilding

    linkflags = rtconfig.M_LFLAGS + ' -e main'
    CPPPATH = GetCPPPATH(BSP_Root, Rtt_Root)

    if rtconfig.PLATFORM == 'cl':
        Env = Environment(TARGET_ARCH='x86')
        Env.Append(CCFLAGS=rtconfig.M_CFLAGS)
        Env.Append(LINKFLAGS=rtconfig.M_LFLAGS)
        Env.Append(CPPPATH=CPPPATH)
        Env.Append(LIBS='rtthread', LIBPATH=BSP_Root)
        Env.Append(CPPDEFINES=GetCPPDEFINES() + ['RTT_IN_MODULE'])
        Env.PrependENVPath('PATH', rtconfig.EXEC_PATH)
    else:
        Env = Environment(tools = ['mingw'],
            AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS + ' -D__EOS__ ',
            CC = rtconfig.CC, CCFLAGS = rtconfig.M_CFLAGS + ' -D__EOS__ ',
            CPPDEFINES = GetCPPDEFINES(),
            CXX = rtconfig.CXX, CXXFLAGS = rtconfig.M_CXXFLAGS + ' -D__EOS__ ',
            AR = rtconfig.AR, ARFLAGS = '-rc',
            LINK = rtconfig.LINK, LINKFLAGS = linkflags,
            CPPPATH = CPPPATH)

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareModuleBuilding(Env, Rtt_Root, BSP_Root)

    #
    # lombo: redirect temporary filess to outdir
    #
    #objs = SConscript(SConscriptFile, variant_dir=app_vdir, duplicate=0)
    outdir = SConscriptFile[(SConscriptFile.rfind('venus') + 6) : ]
    outdir = outdir[0 : outdir.rfind('SConscript') - 1]
    outdir = Rtt_Root + '/../out/build/' + outdir
    objs = SConscript(SConscriptFile, variant_dir=outdir, duplicate=0)

    # build program
    if rtconfig.PLATFORM == 'cl':
        dll_target = TARGET.replace('.mo', '.dll')
        target = Env.SharedLibrary(dll_target, objs)

        target = Command("$TARGET", dll_target, [Move(TARGET, dll_target)])
        # target = dll_target
    else:
        # add ".app" suffix, or the output file will be default "xxx.exe"
        TARGET += "/" + os.path.basename(TARGET) + ".app"
        target = Env.Program(TARGET, objs)

    if hasattr(rtconfig, 'M_POST_ACTION'):
        Env.AddPostAction(target, rtconfig.M_POST_ACTION)

    if hasattr(rtconfig, 'M_BIN_PATH'):
        Env.AddPostAction(target, [Copy(rtconfig.M_BIN_PATH, TARGET)])

def BuildStaticLibrary(TARGET, SConscriptFile, BSP_ROOT = None, RTT_ROOT = None):
    global Env
    global Rtt_Root
    global BSP_Root

    # add comstr option
    AddOption('--verbose',
                dest='verbose',
                action='store_true',
                default=False,
                help='print verbose information during build')

    # build Library in host
    if BSP_ROOT == None and RTT_ROOT == None and not os.getenv('BSP_ROOT'):
        BuildHostLibrary(TARGET, SConscriptFile)
        return

    if RTT_ROOT == None and os.getenv('RTT_ROOT'):
        RTT_ROOT = os.getenv('RTT_ROOT')

    # handle BSP_ROOT and RTT_ROOT
    BuildEnv(BSP_ROOT, RTT_ROOT)

    sys.path = sys.path + [os.path.join(Rtt_Root, 'tools'), BSP_Root]

    # get configuration from BSP
    import rtconfig
    from rtua import GetCPPPATH
    from rtua import GetCPPDEFINES
    from building import PrepareModuleBuilding

    linkflags = rtconfig.M_LFLAGS + ' -e 0'
    CPPPATH = GetCPPPATH(BSP_Root, Rtt_Root)

    if rtconfig.PLATFORM == 'cl':
        Env = Environment(TARGET_ARCH='x86')
        Env.Append(CCFLAGS=rtconfig.M_CFLAGS)
        Env.Append(LINKFLAGS=rtconfig.M_LFLAGS)
        Env.Append(CPPPATH=CPPPATH)
        Env.Append(LIBS='rtthread', LIBPATH=BSP_Root)
        Env.Append(CPPDEFINES=GetCPPDEFINES() + ['RTT_IN_MODULE'])
        Env.PrependENVPath('PATH', rtconfig.EXEC_PATH)
    else:
        Env = Environment(tools = ['mingw'],
            AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS + ' -D__EOS__ ',
            CC = rtconfig.CC, CCFLAGS = rtconfig.M_CFLAGS + ' -D__EOS__ ',
            CPPDEFINES = GetCPPDEFINES(),
            CXX = rtconfig.CXX, CXXFLAGS = rtconfig.M_CXXFLAGS + ' -D__EOS__ ',
            AR = rtconfig.AR, ARFLAGS = '-rc',
            LINK = rtconfig.LINK, LINKFLAGS = linkflags,
            CPPPATH = CPPPATH)

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareModuleBuilding(Env, Rtt_Root, BSP_Root)

    #
    # lombo: redirect temporary filess to outdir
    #
    #objs = SConscript(SConscriptFile)
    outdir = SConscriptFile[(SConscriptFile.rfind('venus') + 6) : ]
    outdir = outdir[0 : outdir.rfind('SConscript') - 1]
    outdir = Rtt_Root + '/../out/build/' + outdir
    objs = SConscript(SConscriptFile, variant_dir=outdir, duplicate=0)

    import building
    name = building.Projects[0]['name']

    # build program
    # the output .a file is default generated:
    #   "scons --slib=source/static" ---> "source/libstatic.a"
    # here we the output .a file inside the source dir:
    #   "scons --slib=source/static" ---> "source/static/libstatic.a"
    #TARGET += "/" + os.path.basename(TARGET)
    TARGET += "/" + name
    target = Env.StaticLibrary(TARGET, objs)

    if hasattr(rtconfig, 'M_POST_ACTION'):
        Env.AddPostAction(target, rtconfig.M_POST_ACTION)

def BuildLibrary(TARGET, SConscriptFile, BSP_ROOT = None, RTT_ROOT = None):
    global Env
    global Rtt_Root
    global BSP_Root

    # add comstr option
    AddOption('--verbose',
                dest='verbose',
                action='store_true',
                default=False,
                help='print verbose information during build')

    # build Library in host
    if BSP_ROOT == None and RTT_ROOT == None and not os.getenv('BSP_ROOT'):
        BuildHostLibrary(TARGET, SConscriptFile)
        return

    if RTT_ROOT == None and os.getenv('RTT_ROOT'):
        RTT_ROOT = os.getenv('RTT_ROOT')

    # handle BSP_ROOT and RTT_ROOT
    BuildEnv(BSP_ROOT, RTT_ROOT)

    sys.path = sys.path + [os.path.join(Rtt_Root, 'tools'), BSP_Root]

    # get configuration from BSP
    import rtconfig
    from rtua import GetCPPPATH
    from rtua import GetCPPDEFINES
    from building import PrepareModuleBuilding

    linkflags = rtconfig.M_LFLAGS + ' -e 0'
    CPPPATH = GetCPPPATH(BSP_Root, Rtt_Root)

    if rtconfig.PLATFORM == 'cl':
        Env = Environment(TARGET_ARCH='x86')
        Env.Append(CCFLAGS=rtconfig.M_CFLAGS)
        Env.Append(LINKFLAGS=rtconfig.M_LFLAGS)
        Env.Append(CPPPATH=CPPPATH)
        Env.Append(LIBS='rtthread', LIBPATH=BSP_Root)
        Env.Append(CPPDEFINES=GetCPPDEFINES() + ['RTT_IN_MODULE'])
        Env.PrependENVPath('PATH', rtconfig.EXEC_PATH)
    else:
        Env = Environment(tools = ['mingw'],
            AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS + ' -D__EOS__ ',
            CC = rtconfig.CC, CCFLAGS = rtconfig.M_CFLAGS + ' -D__EOS__ ',
            CPPDEFINES = GetCPPDEFINES(),
            CXX = rtconfig.CXX, CXXFLAGS = rtconfig.M_CXXFLAGS + ' -D__EOS__ ',
            AR = rtconfig.AR, ARFLAGS = '-rc',
            LINK = rtconfig.LINK, LINKFLAGS = linkflags,
            CPPPATH = CPPPATH)

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareModuleBuilding(Env, Rtt_Root, BSP_Root)

    #
    # lombo: redirect temporary filess to outdir
    #
    #objs = SConscript(SConscriptFile)
    outdir = SConscriptFile[(SConscriptFile.rfind('venus') + 6) : ]
    outdir = outdir[0 : outdir.rfind('SConscript') - 1]
    outdir = Rtt_Root + '/../out/build/' + outdir
    objs = SConscript(SConscriptFile, variant_dir=outdir, duplicate=0)

    # build program
    if rtconfig.PLATFORM == 'cl':
        dll_target = TARGET.replace('.so', '.dll')
        target = Env.SharedLibrary(dll_target, objs)

        target = Command("$TARGET", dll_target, [Move(TARGET, dll_target)])
        # target = dll_target
    else:
        # add ".so" suffix, or the output file will be default "xxx.exe"
        TARGET += "/lib" + os.path.basename(TARGET) + ".so"
        target = Env.Program(TARGET, objs)

    if hasattr(rtconfig, 'M_POST_ACTION'):
        Env.AddPostAction(target, rtconfig.M_POST_ACTION)

def BuildMod(TARGET, SConscriptFile, BSP_ROOT = None, RTT_ROOT = None):
    global Env
    global Rtt_Root
    global BSP_Root

    # add comstr option
    AddOption('--verbose',
                dest='verbose',
                action='store_true',
                default=False,
                help='print verbose information during build')

    # build Library in host
    if BSP_ROOT == None and RTT_ROOT == None and not os.getenv('BSP_ROOT'):
        BuildHostLibrary(TARGET, SConscriptFile)
        return

    if RTT_ROOT == None and os.getenv('RTT_ROOT'):
        RTT_ROOT = os.getenv('RTT_ROOT')

    # handle BSP_ROOT and RTT_ROOT
    BuildEnv(BSP_ROOT, RTT_ROOT)

    sys.path = sys.path + [os.path.join(Rtt_Root, 'tools'), BSP_Root]

    # get configuration from BSP
    import rtconfig
    from rtua import GetCPPPATH
    from rtua import GetCPPDEFINES
    from building import PrepareModuleBuilding

    linkflags = rtconfig.M_LFLAGS + ' -e 0'
    CPPPATH = GetCPPPATH(BSP_Root, Rtt_Root)

    if rtconfig.PLATFORM == 'cl':
        Env = Environment(TARGET_ARCH='x86')
        Env.Append(CCFLAGS=rtconfig.M_CFLAGS)
        Env.Append(LINKFLAGS=rtconfig.M_LFLAGS)
        Env.Append(CPPPATH=CPPPATH)
        Env.Append(LIBS='rtthread', LIBPATH=BSP_Root)
        Env.Append(CPPDEFINES=GetCPPDEFINES() + ['RTT_IN_MODULE'])
        Env.PrependENVPath('PATH', rtconfig.EXEC_PATH)
    else:
        Env = Environment(tools = ['mingw'],
            AS = rtconfig.AS, ASFLAGS = rtconfig.AFLAGS + ' -D__EOS__ ',
            CC = rtconfig.CC, CCFLAGS = rtconfig.M_CFLAGS + ' -D__EOS__ ',
            CPPDEFINES = GetCPPDEFINES(),
            CXX = rtconfig.CXX, CXXFLAGS = rtconfig.M_CXXFLAGS + ' -D__EOS__ ',
            AR = rtconfig.AR, ARFLAGS = '-rc',
            LINK = rtconfig.LINK, LINKFLAGS = linkflags,
            CPPPATH = CPPPATH)

    if not GetOption('verbose'):
        # override the default verbose command string
        Env.Replace(
            ARCOMSTR = 'AR $TARGET',
            ASCOMSTR = 'AS $TARGET',
            ASPPCOMSTR = 'AS $TARGET',
            CCCOMSTR = 'CC $TARGET',
            CXXCOMSTR = 'CXX $TARGET',
            LINKCOMSTR = 'LINK $TARGET'
        )

    PrepareModuleBuilding(Env, Rtt_Root, BSP_Root)

    #
    # lombo: redirect temporary filess to outdir
    #
    #objs = SConscript(SConscriptFile)
    outdir = SConscriptFile[(SConscriptFile.rfind('venus') + 6) : ]
    outdir = outdir[0 : outdir.rfind('SConscript') - 1]
    outdir = Rtt_Root + '/../out/build/' + outdir
    objs = SConscript(SConscriptFile, variant_dir=outdir, duplicate=0)

    # build program
    if rtconfig.PLATFORM == 'cl':
        dll_target = TARGET.replace('.so', '.dll')
        target = Env.SharedLibrary(dll_target, objs)

        target = Command("$TARGET", dll_target, [Move(TARGET, dll_target)])
        # target = dll_target
    else:
        # add ".mod" suffix, or the output file will be default "xxx.mod"
        TARGET += "/" + os.path.basename(TARGET) + ".mod"
        target = Env.Program(TARGET, objs)

    if hasattr(rtconfig, 'M_POST_ACTION'):
        Env.AddPostAction(target, rtconfig.M_POST_ACTION)
