# bz20ru
*说明：*bz20rru的app程序
## 一、编译方法

```
cd bz20ru
source setenv.sh
make
ls app/bin
make install
```
## 二、使用方法
*说明：*包含:
    + bz20rr      **C++控制台程序用于bz20功能测试**
    + bz20test    **C控制台程序用于bz20功能测试**
    + httpserver  **WEB用于测试bz20功能的后台C++服务程序**
    RegisterCommand("info","查看RRU当前工作状态",(void *)boardInfoShow);
