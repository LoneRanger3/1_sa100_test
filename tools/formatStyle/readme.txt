主要有3个功能：
1. 将连续的多行，变成一行，空行里面没有空格
2. 将行尾多余的空格去掉
3. 将\r\n换成\n

执行 $ ./format.py ./tool/gui
就会搜索gui里面的文件夹，进行处理搜索相应的文件，support_file = ["c","h","py","cpp","json"]，
这在format.py里面修改文件处理后缀�
