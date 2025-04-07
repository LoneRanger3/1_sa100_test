from __future__ import print_function
from lombo_setenv import read_update_env, cppcheck_dir
import os, sys, importlib

# read .build and set env, for "scons --menuconfig"
if read_update_env():
	print("read_update_env err!")
	exit(-1)

LOMBO_CPU=os.environ['LOMBO_CPU']
LOMBO_SOC=os.environ['LOMBO_SOC']
LOMBO_BOARD=os.environ['LOMBO_BOARD']
BSP_CFG_SOC=os.environ['BSP_CFG_SOC']
TOP_DIR=os.environ['TOP_DIR']
BSP_DIR=os.environ['BSP_DIR']
KERN_DIR=os.environ['KERN_DIR']

#
# continue building
#
board_cfg = BSP_CFG_SOC + '/rtconfig_' + LOMBO_CPU + LOMBO_SOC + '.py'
#print('board_cfg is ' + board_cfg)

sys.path.append(os.path.dirname(board_cfg))
fname = os.path.splitext(os.path.basename(board_cfg))[0]
globals().update(vars(importlib.import_module(fname)))
sys.path.pop()
