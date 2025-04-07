# The Library builder of eos #

## Introduction ##

As you know, there are two type of Libraries,

* Shared library
* Static libra8ry

They are compose of functions, and provide these APIs to other programs.

## Directory ##
.
├── tools
├── readme.txt
├── SConscript
├── SConstruct
├── source
│   ├── dynamic
│   │   ├── crt -> ../../../../../components/cplusplus
│   │   ├── inc
│   │   ├── lib.c
│   │   ├── SConscript
│   │   └── src
│   ├──	static
│   │   ├── inc
│   │   ├── SConscript
│   │   └── src
│   └── sample
│       ├── lib_sample
│       └── slib_sample
│           ├── Global
│           └── Local
└── libslib
    ├── inc
    ├── libslib.a
    └── SConscript

* tools		Just the builder tools
* SConscript	The libraries SConscript, it just works in the whole BSP build.
* SConstruct	The library builder SConstruct, it's used for making library.
* source	All library source code should be saved in this directory.
*   dynamic	A sample of shared library, just for the mode of dynamic.
*   static	A sample of static library, just for the mode of static.
*   sample	It shows how to call these libraries
* libslib	It is the output of source/static, which was builded by tool/ua.py.

## Build Library ##

1) config menuconfig
run command "scons --menuconfig" on your BSP directory:
--------------------------------------------------------------------------------------------
   RT-Thread Components  --->
       POSIX layer and C standard library  --->
	       [*] Enable libc APIs from toolchain
	       [*] Enable dynamic module with dlopen/dlsym/dlclose feature
--------------------------------------------------------------------------------------------
If need C++ compiler,please open C++ features:
--------------------------------------------------------------------------------------------
   RT-Thread Components  --->
       C++ features  --->
	       [*] Support C++ features
--------------------------------------------------------------------------------------------

2) run the command under your BSP directory:
	scons --target=ua -s

3) Finally, you can build the library in 'libraries' directory, for example:
To build static library:
	scons --slib=demo/static
You can get the library named "libstatic.a" in 'demo/static' directory.
It was name by the name of DefineGroup in SConscript.

To build shared library:
	scons --lib=demo/dynamic
You can get the library named "libdynamic.so" in 'demo/dynamic' directory.
It was name by the name of DefineGroup in SConscript.
Note: if your shared library need C++, you have to add C++ runtime wrapper.

To build application:
These are 2 app demos:
(1) application which call dynamic library:
	scons --app=demo/app/dynamic
You can get the application "dynamic.mo" in 'demo/app/dynamic' directory.
The dynamic.mo depends on libdynamic.so to run.

(2) application which compile with static library:
	scons --app=demo/app/static
You can get the application "static.mo" in 'demo/app/static' directory.
The compiling depends on libstatic.a.

