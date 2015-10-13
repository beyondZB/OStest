#lofsutil

##概述
lofsutil是一个操作磁盘设备或者磁盘镜像，创建管理LOFS分区的工具

##安装
lofsutil安装很简单，首先安装kake，然后在lofsutil源代码根目录下执行如下命令：

    kake init c
    kake

如果没有报告编译错误就说明构建完成。接下来只需要进入target目录即可。里面会有一个生成的可执行文件，名为"lofsutil-版本号"，版本号目前为0.0.1，执行以下命令可以获得帮助：

    ./lofsutil-0.0.1

##使用说明
###获取帮助
若要获取在线帮助，可以执行如下命令：

    lofsutil -h

或者

    lofsutil

###清除磁盘
lofsutil可以清除设备文件或者镜像文件：

    lofsutil -c <镜像文件名>

比如想要清除名为a.img的镜像文件，输入：

    lofsutil -c a.img

###格式化分区
lofsutil可以在设备或镜像文件中建立分区，并建立LOFS文件系统：

    lofsutil -f <镜像文件名>

比如想要在a.img镜像文件上建立文件系统，输入：

    lofsutil -f a.img

###获取分区信息
lofstutil可以获取设备或镜像文件上已创建分区的概述信息：

    lofsutil -m <镜像文件名>

比如，想获取a.img镜像文件上创建的文件系统的信息，输入：
    
    lofsutil -m a.img

###将本地文件放入分区
如果设备或镜像文件中已经创建分区，可以将指定本地文件复制到分区中：

    lofsutil -p <本地文件名> <镜像文件名>

比如，想将本地文件loader复制到a.img的分区中，输入：
    
    lofsutil -p loader a.img

###从分区中获取文件
如果设备或镜像文件中已经创建分区，可以将分区中的指定文件复制到本地：

    lofsutil -g <欲获取的文件名> <镜像文件名>

比如，想将a.img的分区中的文件loader复制到本地，输入：
    
    lofsutil -g loader a.img

###删除文件
如果设备或镜像文件中已经创建分区，可以删除分区中的指定文件：

    lofsutil -r <欲删除的文件名> <镜像文件名>

比如，若想删除a.img中的文件loader，输入：
    
    lofsutil -r loader a.img

###写入mbr
将指定mbr程序写入设备或镜像文件的引导扇区:
    
    lofsutil -b <欲写入的mbr程序> <镜像文件名>

比如， 若想将引导程序mbr写入a.img的引导扇区，输入：

    lofsutil -b mbr a.img

###写入嵌入式代码
将指定嵌入式代码写入设备或镜像文件的嵌入式扇区:
    
    lofsutil -e <欲写入的嵌入式代码> <镜像文件名>

比如， 若想将嵌入式代码findLoader写入a.img的嵌入式扇区，输入：

    lofsutil -e findLoader a.img