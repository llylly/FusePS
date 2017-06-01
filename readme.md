### FUSE Proj - PS Module

*Authored by llylly*

+ Development environment: MAC OS X 10.12

+ Form: Qt C++ Project

+ Library Dependencies:

  + FUSE for OS X
  + Qt Runtime Library

+ Run commmand:

  `./FusePS -d [-PSBuf {Buffer Folder Name}] {Folder Name}`

  I still don't know why it needs `-d` but it really can't work without it...

  -PSBuf is an additional optional argument to specify the buffer folder. It can either exist or non-exist, but when it non-exists, it should be under a existed folder (so I can use 'makedir' syscall to create it). By default, the buffer folder name is `PSBuf`.

  {Folder Name} and all other arguments are passed to FUSE engine.

+ Usage:

  Drug pictures to the folder you like, and just see what happened to your pic.

  Effect parameters can be edited in `config.txt` in each folder, if exists.

+ Supported effects:

  + Graying (灰度)
  + Scaling (縮放)
  + Blur (模糊，實現為高斯模糊)
  + Mosaic (馬賽克)
  + Brightness (亮度，實現為Gamma調節)
  + Contrast (對比度，用兩個參數具體確定)

  Each effect corresponds to a folder in '/PS'

+ Supported pics:

  jpg / jpeg / bmp / png

  Unsupported pics cannot be drugged in XD