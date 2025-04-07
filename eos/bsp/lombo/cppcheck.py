import os
import sys
import rtconfig

err_code = 1
# Use --force to check all configurations.
# Cppcheck needs around 35 hours to go though the complete code
# (with force probably significantly longer).
#checkparams = ['--quiet', '--error-exitcode=%d'%(err_code), '--enable=all', '--force']
# By default only error messages are shown.
checkparams = ['-j8', '--quiet', '--error-exitcode=%d'%(err_code)]
# warning messages exit.
#checkparams = ['-j8', '--quiet', '--error-exitcode=%d'%(err_code), '--enable=warning']

def run(cmd):
    code = err_code
#   Run a command and decipher the return code. Exit by default.
    print str(cmd)
    res = os.system(cmd)
    # Assumes that if a process doesn't call exit, it was successful
    if (os.WIFEXITED(res)):
        code = os.WEXITSTATUS(res)
    return code

# Utility productions
def Utility(env, target, source, action):
    target = env.Command(target=target, source=source, action=action)
#    env.AlwaysBuild(target)
#    env.Precious(target)
    return target

def _check(env, target, include, sources, group, params):
    return Utility(env, target, sources, [
            '@echo "\033[1;35m>>>>>>>>>>>> Running cppcheck on \033[0m\033[1;31m%s \033[0m\033[1;35m>>>>>>>>>>>>\033[0m\\ndir =\033[32m %s \033[0m"'%(group['name'], group['path']),
            '-cppcheck %s %s %s 2> $TARGET'%(" ".join(params), " ".join(include), " ".join(sources)),
            ])

def cppcheckPro(env, group):
        #print ">>>>>>>>>>>> cppcheck project:\033[1;35m %s \033[0m dir=%s" %(group['name'], group['path'])
        include = list()
        sources = list()
	path = os.path.abspath(os.path.join(os.getcwd(), "../../.."))+'/out/build/cppcheck_report'
	report = '%s/cppcheck_report_%s.txt' % (path, group['name'])

	if os.path.exists(report):
		os.remove(report)
	elif not os.path.exists(path):
		os.mkdir(path)

        include.append('%s'% (rtconfig.EX_INC))
        for inc in group['CPPPATH']:
                include.append('-I%s'% (inc))
#       for src in group['src']:
#               sources.append(str(src))
        sources.append(group['path'])

#        env.Alias('cppcheck', _check(env, report, include, sources, group, checkparams))
	print('\033[1;35m>>>>>>>>>>>> Running cppcheck on \033[0m\033[1;31m%s \033[0m\033[1;35m>>>>>>>>>>>>\033[0m\ndir =\033[32m %s \033[0m'%(group['name'], group['path']))
	res = run('cppcheck %s %s %s 2> %s'%(" ".join(checkparams), " ".join(include), " ".join(sources), report))
	if res != 0:
		print ("\033[1;31mError: cppcheck error code[%s], read the detail on :\033[0m" %(res))
		print ("\033[1;35m%s\033[0m" %(report))
		sys.exit(res)

# return results if path is in the directory
def is_subdir(path, directory):
    path = os.path.realpath(path)
    directory = os.path.realpath(directory)
    relative = os.path.relpath(path, directory)
    return not (relative == os.curdir or relative.startswith(os.pardir));

def cppcheckAll(env, Projects):
#       print ">>>>>>>>>>>> cppcheck All projects:\033[1;35m %s \033[0m" %(env['project'])
        include = list()
        sources = list()
	temp = ""

        for group in Projects:
		# ignore root path
		if(group['name'] == 'Kernel'):
			continue

		# ignore third party code
		if(group['name'] == 'gui' or group['name'] == 'lvgl'):
                        continue

		# ignore sun directory
		if(temp != "" and is_subdir(group['path'], temp)):
			#print ("Pass name:%s" %(group['name']))
			#print ("path:%s" %(group['path']))
			continue
		temp = group['path']

		#print ("name:%s	path:%s temp:%s" %(group['name'], group['path'], temp))
                cppcheckPro(env, group)

def cppcheck(env, Projects):
	res = 1;
        print("\033[1;33;44m================= Scons cppcheck code ! =================\033[0m")
        is_cppcheck = env.WhereIs('cppcheck')
        if(is_cppcheck == None):
                print('\033[1;35m!!! cppcheck not found on this system.  Check if cppcheck is installed and in your PATH.\033[0m')
                sys.exit(1)

#        print ("Cppcheck Project:\033[1;35m %s \033[0m" %(rtconfig.cppcheck_dir))
	paths = rtconfig.cppcheck_dir.split(' ')
	ignores = [os.getcwd(), os.path.abspath(os.path.join(os.getcwd(), "../../.."))]

	if paths[0] == ignores[1]:
                cppcheckAll(env, Projects)
        else:
		for path in paths:
			res = 2
			tmp = path
			while res == 2:
				#print ("\033[1;32mtmp >> %s\033[0m" %(tmp))
				for group in Projects:
					#print ("name:%s	path:%s" %(group['name'], group['path']))
					if group['path'].endswith(tmp):
						#print ("\033[1;35mstart test >> %s\033[0m" %(group['path']))
						cppcheckPro(env, group)
						res = 0
						break
				tmp = os.path.split(tmp)[0]
				if(ignores[0].endswith(tmp) or ignores[1].endswith(tmp)):
					print ("\033[1;36mIgnore cppcheck with the missing path >> %s\033[0m" %(path))
					res = 0;
					break
