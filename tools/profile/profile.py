#!/usr/bin/python
#coding=utf-8
import os
import sys
import binascii
import commands
from time import strftime, localtime

FIND_FUNC_CMD = 'arm-none-eabi-addr2line'
ELF_FILE = 'eos/bsp/lombo/rtthread-lombo.elf'
# ELF_FILE_NAME = 'rtthread-lombo.elf'

DATA_LEN = 16
READ_DATA_LEN = 1024 * 16 * DATA_LEN
USED = 1
PROF_CSV_FILE_NAME = 'profile.csv'

# for store record information
class ProfRecord:
	number = 1

	def __init__(self, fn, time):
		self.fn = fn
		self.time = time

	def add_time(self, t):
		self.time += t
		self.number += 1

	def log(self):
		print('%s: \ttime: %d, \tnumber: %d\t' % (self.fn, self.time, self.number))

# get the rtthread-lombo.elf path
def elf_file_path():
	cwd = os.getcwd()
	dir_name = 'venus'

	index = cwd.find(dir_name)
	if index < 0:
		return ''

	d = cwd[0: index + len(dir_name)]
	elf_p = os.path.join(d, ELF_FILE)
	return elf_p

def parse_data(data):
	l = len(data)
	if l != DATA_LEN:
		print('Error: length of data must be ' + DATA_LEN)
		return ('', 0, 0)

	# 0-3bit: function address; 4-7bit: use; 8-15bit: time
	addr = data[0:4]
	use = data[4:8]
	time = data[8:l]

	# reverse bit
	addr = addr[::-1]
	use = use[::-1]
	time = time[::-1]

	# convert to string
	addr_str = addr.encode('hex')
	use_str = use.encode('hex')
	time_str = time.encode('hex')

	address = '0x' + addr_str
	use_i = int(use_str, 16)
	time_i = int(time_str, 16)
	return (address, use_i, time_i)

def addr_2_func(addr):
	# translate address to function name
	if use != USED:
		return ''

	# check elf file exist
	elf = elf_file_path()
	exist = os.path.exists(elf)
	if exist != True:
		print('error: ELF file: ' + elf + ' not exist')
		sys.exit()

	cmd = FIND_FUNC_CMD + ' -e ' + elf + ' -f ' + addr
	status, output = commands.getstatusoutput(cmd)

	if status != 0:
		print(FIND_FUNC_CMD + ' status: ' + str(status))
		return ''

	# output content contain '??' if the ELF file mismatch the bin file
	index = output.find('??')
	if index >= 0:
		print('get function name error, may be the ELF file mismatch the binary data file')
		sys.exit()

	# arr contain 2 element, function name and file line number
	arr = output.splitlines()
	if len(arr) != 2:
		print('Error: output format invalid')
		return ''

	fn = arr[0]
	return fn

def save_2_dict(dict, fn, time):
	if fn in dict:
		record = dict[fn]
		record.add_time(time)
	else:
		record = ProfRecord(fn, time)
		dict[fn] = record

# save dictionary data to csv file
def save_csv(dict):
	print('save data to csv file...')
	sep = ','
	t = strftime('%Y%m%d_%H%M%S_', localtime())
	f_name = t + PROF_CSV_FILE_NAME
	try:
		f = open(f_name, 'w')
		title = 'function_name%stime%snumber\n' % (sep, sep)
		f.write(title)

		for key in dict:
			record = dict[key]
			text = '%s%s%d%s%d\n' % (record.fn, sep, record.time, sep, record.number)
			f.write(text)
	finally:
		if f:
			f.close()
			print('save csv file finished: %s' % (f_name))


if len(sys.argv) < 2:
	print('please enter the input file')
	sys.exit()

source = sys.argv[1]
exist = os.path.exists(source)
if exist != True:
	print('error: ' + source + ' not exist')
	sys.exit()

# use dictionary to save profile record info, key is function name
dict = {}

# save the address to function name
func_cache = {}

# open the binary file and process it's data
try:
	sz = os.path.getsize(source)
	print('%s size: %d' % (source, sz))
	if sz % DATA_LEN != 0:
		print('error: size of file invalid')
		sys.exit()

	f = open(source, 'rb')
	data = f.read(DATA_LEN)

	total = 0	# current processed data size
	progress = 0	# parse data progress

	print('parsing binary file data ...')
	while True:
		l = len(data)
		if l == 0:
			print('End of file')
			break

		if l != DATA_LEN:
			print('length of data invalid')
			break

		addr, use, time = parse_data(data)

		# find in dictionary cache first, if not exist, store it
		fn = ''
		if addr in func_cache:
			fn = func_cache[addr]
		else:
			fn = addr_2_func(addr)
			func_cache[addr] = fn

		save_2_dict(dict, fn, time)

		# if time > 10:
		# 	print('address: ' + addr + ', use: ' + str(use) + ', time: ' + str(time))
		# 	print(fn)

		# update process progress
		total += l
		new_pg = int(total * 1.0 / sz * 100)
		if progress != new_pg:
			progress = new_pg
			print('parse binary file data: %d%%' % (progress))

		data = f.read(DATA_LEN)

finally:
	if f:
		print('parse data finished and close file')
		f.close()

print('========== profile time ========')
print('total count: %d' % (len(dict)))
# for key in dict:
# 	record = dict[key]
# 	record.log()

# save dictionary data to csv file
save_csv(dict)
