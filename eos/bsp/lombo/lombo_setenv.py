import os
import sys

#
# read .build and set env, for the case that env was not updated by bash
#   eg: scons --menuconfig
#
def read_update_env():
	build_dir = os.path.abspath(os.path.join(
		os.path.dirname(__file__), "../../../.build"))
	if not os.path.isfile(build_dir):
		print(build_dir + ' not exist, please \"./buld.sh config\"!')
		return -1
	file = open(build_dir)
	for line_str in file:
		line_str = line_str.strip('\n')
		key = line_str[: line_str.find('=')]
		key = key[key.rfind(' ') + 1 :]
		val = line_str[line_str.rfind('=') + 1 :]
		if key.strip() and val.strip() :
			os.environ[key] = val
	return 0

def get_compile_option(option):
        for i in range(1, len(sys.argv)):
                str = sys.argv[i]
                if (str.find(option) != -1):
                        return str[len(option):]
        return 'None'

#
# get the cppcheck dir
#
cppcheck_dir = get_compile_option('cppcheck_dir=')
