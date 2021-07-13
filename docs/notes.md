# 笔记

## 目录传送门

- [从零开始手敲次世代游戏引擎（序）](https://zhuanlan.zhihu.com/p/28587092)
- [从零开始手敲次世代游戏引擎（一）](https://zhuanlan.zhihu.com/p/28589792)
- [从零开始手敲次世代游戏引擎（二）](https://zhuanlan.zhihu.com/p/28598462) toolchain
- [从零开始手敲次世代游戏引擎（三）](https://zhuanlan.zhihu.com/p/28611487) toolchain
- [从零开始手敲次世代游戏引擎（四）](https://zhuanlan.zhihu.com/p/28615565) 简单的设计和规划
- [从零开始手敲次世代游戏引擎（五）](https://zhuanlan.zhihu.com/p/28619982) 项目架子
- [从零开始手敲次世代游戏引擎（六）](https://zhuanlan.zhihu.com/p/28633100) 图形相关的开篇，开始图形支线
- [从零开始手敲次世代游戏引擎（七）](https://zhuanlan.zhihu.com/p/28641118) windows 窗体
- [从零开始手敲次世代游戏引擎（八）](https://zhuanlan.zhihu.com/p/28665581) linux 窗体 xcb
- [从零开始手敲次世代游戏引擎（九）](https://zhuanlan.zhihu.com/p/28679304) 在已实现的窗体中增加绘制操作
- [从零开始手敲次世代游戏引擎（十）](https://zhuanlan.zhihu.com/p/28711014) 从cpu绘制切换至gpu绘制，使用的是windows的dx，进行2d绘制；（暂无运行环境，先跳过）
- [从零开始手敲次世代游戏引擎（十一）](https://zhuanlan.zhihu.com/p/28719057) 使用Direct 3D进行3D绘制；（暂无运行环境，先跳过）
- [从零开始手敲次世代游戏引擎（十二）](https://zhuanlan.zhihu.com/p/28756646) linux 环境使用opengl； （暂无直接运行环境，使用macOS进行接入）
- [从零开始手敲次世代游戏引擎（MacOS特别篇）](https://zhuanlan.zhihu.com/p/30721683) mac 接入xcb及使用opengl

---

- `#pragma once` 用于防止header重复展开，等效于复杂的 `#ifndef __INTERFACE_H__`写法。
- `nm` llvm symbol table dumper
- `otool` the otool-compatible command line parser for llvm-objdump

## 创建窗体

### win

TBD

### linux

[X.Org Server](https://en.wikipedia.org/wiki/X.Org_Server) 主要概念：

- x window system (X11)
- X.Org Server is an implementation of the X Window System display server.
- Implementations of the client side of the protocol are available e.g. in the form of Xlib
  and [XCB(X protocol C-language Binding)](https://en.wikipedia.org/wiki/XCB). We use xcb in demo.
- [XQuartz](https://www.xquartz.org/) is a series of patches from Apple Inc. The XQuartz project is an open-source
  effort to develop a version of the X.Org X Window System that runs on macOS.

Both xcb and xquartz can be installed through Brew in macOS. Check the `CMakeLists.txt` file in `Platform/Dawrin` for
linking details.

### mac

OpenGL consists of two parts:

- GL, the core
- GLU, has a bunch of convenience functions.

What is glx? We are trying to use Xquartz to implement an opengl demo, so we need GLX to access abilities of OpenGL in X
Window System.

- [GLX](https://en.wikipedia.org/wiki/GLX) (OpenGL Extension to the X Window System), is an extension to the X Window
  System core protocol providing an interface between OpenGL and the X Window System as well as extensions to OpenGL
  itself. It enables programs wishing to use OpenGL to do so within a window provided by the X Window System. GLX
  distinguishes two "states": indirect state and direct state.
- And why xlib, [read OpenGL, Xlib and GLX](https://xcb.freedesktop.org/opengl/#index1h1)

  > Although writing a OpenGL application on X Windows using pure XCB is not possible,
  > it is possible to use a XCB-based Xlib implementation to configure a rendering context in a hybrid XCB/Xlib application. Xlib is used with the GLX functions while XCB can be used for everything else. You do get all the advantages of using XCB, but you can’t get entirely rid of Xlib.

  [sample code](https://xcb.freedesktop.org/opengl/#index5h1)

---

### 修复glxversion获取错误的问题

错误如下：

```shell
/Users/mob/Code/workspace/MakiEngine/cmake-build-debug/Platform/Darwin/hello_opengl
Invalid GLX version
debug 1: display 0x7fec5e00a600 glXQueryVersion 0x10649f26a
glXQueryVersion result: 0

Process finished with exit code 1
```

刚开始cmakelists中配置如下，首先从`/url/local/{include,lib}`中获取xcb、x11相关头文件、及链接库；
原希望从`/url/local/{include,lib}`中链接GL、GLU，还依赖`GL/glx.h`，但这里并没有找到GLX的头文件；

因为GLX是X Server的相关API，所以猜测在`/opt/X11/{include,lib}`中，于是从这里找到了`GL/*`相关完整头文件和链接库，从`/opt/X11`中获取相关lib的头文件和链接库，推测从/usr/local中链接不完整或者版本不对？注释掉`link_directories(/usr/local/lib)`，让链接过程从`/opt/X11/lib`中查找，即可正确运行。

只从`/opt/X11`中获取相关头文件和链接库即可。

``` cmake
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

include_directories(/opt/X11/include)
link_directories(/opt/X11/lib)
```

延伸：

尝试进一步定位原因，过程中用到了两个之前不知道的工具。
`/usr/local/lib`中，是有glx的库的，不清楚为啥这个库能链接能编译，编译完之后运行结果不符合预期，一下是一些分析的方法：

可以**通过使用`otool`查看dylib的版本信息和兼容信息**：

```shell
➜ otool -L /usr/local/lib/libxcb-glx.dylib
/usr/local/lib/libxcb-glx.dylib:
	/usr/local/opt/libxcb/lib/libxcb-glx.0.dylib (compatibility version 1.0.0, current version 1.0.0)
	/usr/local/Cellar/libxcb/1.14_1/lib/libxcb.1.dylib (compatibility version 3.0.0, current version 3.0.0)
	/usr/local/opt/libxau/lib/libXau.6.dylib (compatibility version 7.0.0, current version 7.0.0)
	/usr/local/opt/libxdmcp/lib/libXdmcp.6.dylib (compatibility version 7.0.0, current version 7.0.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1281.100.1)
	
➜ otool -L /opt/X11/lib/libxcb-glx.dylib
/opt/X11/lib/libxcb-glx.dylib:
	/opt/X11/lib/libxcb-glx.0.dylib (compatibility version 1.0.0, current version 1.0.0)
	/opt/X11/lib/libxcb.1.dylib (compatibility version 3.0.0, current version 3.0.0)
	/opt/X11/lib/libXau.6.dylib (compatibility version 7.0.0, current version 7.0.0)
	/usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1292.120.1)
```

比较奇怪的是同样是`/usr/lib/libSystem.B.dylib`，current version是有差异的，这可能是原因，不确定；

另外，**可以通过`nm`查看dylib中的符号**，确定API：

```shell
➜ nm /usr/local/lib/libxcb-glx.dylib | grep version
0000000000005e0b T _xcb_glx_query_version
000000000000c130 s _xcb_glx_query_version.xcb_req
0000000000005ecc T _xcb_glx_query_version_reply
0000000000005e6d T _xcb_glx_query_version_unchecked
000000000000c148 s _xcb_glx_query_version_unchecked.xcb_req
0000000000007c80 T _xcb_glx_set_client_info_2arb_gl_versions
0000000000007c96 T _xcb_glx_set_client_info_2arb_gl_versions_end
0000000000007c8a T _xcb_glx_set_client_info_2arb_gl_versions_length
00000000000078a2 T _xcb_glx_set_client_info_arb_gl_versions
00000000000078b7 T _xcb_glx_set_client_info_arb_gl_versions_end
00000000000078ac T _xcb_glx_set_client_info_arb_gl_versions_length

➜ nm /opt/X11/lib/libxcb-glx.dylib | grep version
0000000000001e0b T _xcb_glx_query_version
0000000000008150 s _xcb_glx_query_version.xcb_req
0000000000001ecc T _xcb_glx_query_version_reply
0000000000001e6d T _xcb_glx_query_version_unchecked
0000000000008168 s _xcb_glx_query_version_unchecked.xcb_req
0000000000003c80 T _xcb_glx_set_client_info_2arb_gl_versions
0000000000003c96 T _xcb_glx_set_client_info_2arb_gl_versions_end
0000000000003c8a T _xcb_glx_set_client_info_2arb_gl_versions_length
00000000000038a2 T _xcb_glx_set_client_info_arb_gl_versions
00000000000038b7 T _xcb_glx_set_client_info_arb_gl_versions_end
00000000000038ac T _xcb_glx_set_client_info_arb_gl_versions_length
```

从这儿可以看到，两者都存在query_version的api，可能不是问题所在。

最终没有得出什么结论，暂时只通过保证从一致的位置获取X11相关头文件和链接库，先跳过这个问题，后续回头再看有没有什么头绪。

### 无法正常退出xcb opengl的demo

```shell
Process finished with exit code 134 (interrupted by signal 6: SIGABRT)
```

demo的结束dispose逻辑貌似有问题，不能正常地exit，暂时不处理
