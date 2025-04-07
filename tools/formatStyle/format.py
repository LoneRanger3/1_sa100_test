#!/usr/bin/env python
import os
import sys

def make(filename):
    f = open(filename, "rb")
    lines = f.readlines()
    f.close()

    lines_new = []
    cnt = 0
    for l in lines:
        line = ""
        for i in range(len(l)-1, -1, -1):
            if len(l) == 1 and l == '\n':
                break
            elif len(l) == 1 and l <> '\n':
                line += l
                break
            elif len(l) == 2 and l == '\r\n':
                break
            elif len(l) == 2 and l <> '\r\n':
                line += l
                break
            elif l[i] <> ' ' and l[i] <> '\t' and l[i] <> '\r' and i <> len(l)-1:
                line +=l[0:i+1]
                break
        if line.find('\n') == -1:
            line += "\n"
        if line == "\n":
            cnt += 1
        else:
            cnt = 0

        if cnt < 2:
            lines_new.append(line)

    f = open(filename, "wb")
    f.writelines(lines_new)
    f.close()

if __name__ == '__main__':
    root = ".\\"
    argv = sys.argv
    argc = len(argv)
    if argc == 2:
        root = argv[1]

    support_file = ["json"]
    for roots, dirs, files in os.walk(root):
        for f in files:
            path = os.path.join(roots, f)
            ext = path.split(".")[-1]
            if ext.lower() in support_file:
                print path
                make(path)
    
#    print os.getcwd()
#    for name in os.listdir(root):
#        path = os.path.join(root, name)
#        ext = path.split(".")[-1]
#        if (os.path.isdir(path) == False) and (ext.lower() in support_file):
#            print path
#            make(path)

