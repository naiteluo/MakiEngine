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

## 创建窗体

### win

TBD

### linux

[X.Org Server](https://en.wikipedia.org/wiki/X.Org_Server) 主要概念：

- x window system (X11)
- X.Org Server is an implementation of the X Window System display server.
- Implementations of the client side of the protocol are available e.g. in the form of Xlib and [XCB(X protocol C-language Binding)](https://en.wikipedia.org/wiki/XCB). We use xcb in demo.
- [XQuartz](https://www.xquartz.org/) is a series of patches from Apple Inc. The XQuartz project is an open-source effort to develop a version of the X.Org X Window System that runs on macOS.

Both xcb and xquartz can be installed through Brew in macOS.
Check the `CMakeLists.txt` file in `Platform/Dawrin` for linking details.

