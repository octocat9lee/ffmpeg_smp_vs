# ffmpeg_smp_vs

## 使用步骤
+ 安装 YASM 与 NASM
+ 克隆仓库到本地目录
+ 从百度网盘下载其他的依赖库  `ffmpeg_smp_vs_3rdparty_source.zip`
+ 解压  `ffmpeg_smp_vs_3rdparty_source.zip`  到  `source`  路径下
+ 进入 `ffmpeg_smp_vs\source\FFmpeg\SMP` 目录，使用 `Visual Studio 2017` 打开 `ffmpeg_deps.sln`
+ 生成解决方案

## 调试说明
+ 项目的 debug 配置好像有问题，如果调试的时候找不到 ffmpegd.exe，根据[ShiftMediaProject具体使用](https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/shift-media-project2.html)中的配置进行调试配置即可。

## Reference
[安装 YASM 与 NASM](https://www.cnblogs.com/suiyek/p/15669562.html#%E4%BA%8C%E5%AE%89%E8%A3%85-yasm-%E4%B8%8E-nasm)
[ShiftMediaProject项目介绍](https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/shift-media-project.html)
[ShiftMediaProject具体使用](https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/shift-media-project2.html)
[Windows下搭建FFmpeg开发调试环境](https://www.cnblogs.com/suiyek/p/15669562.html)
[使用 myys2 与 msvc 编译 Ffmpeg](https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/msys2-msvc.html)
