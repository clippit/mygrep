# 一个简单的 grep 实现

## 主要特性
* 使用正则表达式进行匹配，可以直接传入正则，也可以跟在`-e`选项后
* 可以对多个文件同时进行匹配，直接传入多个文件名即可
* 支持 Shell 管道操作符，可以对标准输入流进行匹配
* 支持反向匹配，即输出不匹配的行
* 可以输出匹配的行数，支持忽略大小写的选项

### 具体用法
    A tiny grep impletment as the homework of Linux Programming
    Usage: mygrep [OPTION]... PATTERN [FILE]...
    Search for PATTERN in each FILE or standard input.
    Options:
    -h            Show this help
    -E            Interpret PATTERN as an extended regular expression.
    -e PATTERN    Use PATTERN as the pattern. This can be used to protect a
                  pattern beginning with a hyphen (-).
    -H            Print the filename for each match.
    -i            Ignore case distinctions.
    -n            Print line number with output lines.
    -v            Select non-matching lines.

## 如何编译
已经预先准备好 Makefile，直接`make`即可。如果要编译调试版本，使用`make DEBUG=1`。清除编译好的文件，使用`make clean`。

## 示例
### 单个文件匹配
    $ ./mygrep app testfile
    apple
    pineapple
    apple123
    apple-

### 忽略大小写并显示文件名和行数
    $ ./mygrep -iHn app testfile
    testfile:1:apple
    testfile:2:APple
    testfile:3:pineapple
    testfile:4:apple123
    testfile:5:apple-
    testfile:6:-aPPle

### 使用`-e`以使用`-`符号开头的正则表达式
    $ ./mygrep -e -a testfile
    -aPPle

### 匹配多个文件
    $ ./mygrep app testfile testfile2
    testfile:apple
    testfile:pineapple
    testfile:apple123
    testfile:apple-
    testfile2:not a apple
    testfile2:http://www.apple.com

### 反向匹配多个文件
    $ ./mygrep -vn app testfile testfile2
    testfile:2:APple
    testfile:6:-aPPle
    testfile:7:hello
    testfile2:1:aPPLe
    testfile2:4:Linux

### 对一个目录下的所有文件进行查找
    $ ./mygrep ENOENT /usr/include/sys/*
    /usr/include/sys/errno.h:#define    ENOENT      2       /* No such file or directory */
    /usr/include/sys/kern_event.h:  @discussion Lookup the vendor code for the specified vendor. ENOENT will be


## 实现细节
本程序使用了 GNU C library 中的正则库，读取命令行参数时使用`getopt`函数。

程序返回值有三种：

* 0: 匹配成功
* 1: 匹配失败
* 2: 遇到错误

遇到错误时，程序会显示错误描述和错误代码。

不支持目录下递归遍历所有子目录文件的grep操作，遇到目标文件是个目录时，会输出 _is a directory_ 的警告。

