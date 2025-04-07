import os
import sys

def _get_current_dirs(cur_path, exclude):
	# get all directories int the current path except the specified file
	dirs = []
	for f in os.listdir(cur_path):
		if f == exclude:
			continue

		fp = os.path.join(cur_path, f)
		if os.path.isdir(fp):
			dirs.append(f)
	return dirs

def _check_file(file, arr):
	for f in arr:
		if f in file:
			return True

def _check_dir(dir, arr):
	for f in os.listdir(dir):
		p = os.path.join(dir, f)
		if _check_file(f, arr):
			if f != "include.h":
				print("woooowwwwwww! -> filename:" + f + ", path: " + p)

		if os.path.isdir(p):
			_check_dir(p, arr)

# return directories string join with ',' for gcc compile paramaters
def exclude_dirs():
	specified_dirs = ['profile']
	prj_name = '/eos/'
	cwd = os.getcwd()

	index = cwd.rfind(prj_name)
	if index < 0:
		print('warn: get profile exclude directory failed')
		print('current cwd:' + cwd)
		return ''

	# root of venus project
	root = cwd[0: index]
	result = []

	# no include dir in eos/bsp/lombo
	exclude_dir = "eos/bsp/lombo"
	arr = exclude_dir.split("/")

	'''
	for loop:
		1 loop: get all directories in /venus except 'eos' dir
		2 loop: get all directories in /venus/eos except 'bsp' dir
		3 loop: get all directories in /venus/eos/bsp except 'lombo' dir
	'''
	for d in arr:
		dirs = _get_current_dirs(root, d)
		result.extend(dirs)
		root = os.path.join(root, d)

	result.extend(specified_dirs)

	# print("check directory:")
	# _check_dir(root, result)

	result_str = ",".join(result)
	return result_str
