# Franca IDL
The francaidl support a Franca IDL parser which can parse IDL files and build complete model trees. And it also support a IDL code generation tool that can generate various types of code based on the model trees.  
It is independent of the operating system, can be compatible with multiple operating systems, and can currently run on the BST-OS linux, Windows, Ubuntu 18.04, Ubuntu 20.04 or Ubuntu 22.04 operating system.

## Features
- IDL code generator : A C++ code generator, which parsing the given IDL files and then generating the appropriate  code according to the specified type or deployment in the IDL files.

## Usage

### IDL code generator
```
usage: bst_idl_code_gen -h
 -h,--help   display help of available console commands

Command: Version Information
usage: bst_idl_code_gen [-a | -p]  -v
 -a,--all       print code generator and plug-in versions
 -p,--plugins   print plug-in versions
 -v,--version   print code generator version

Command: CommonAPI Core Code Generation
usage: bst_idl_code_gen [[-t TYPE] [-d DIR] [-l LIC] [-s] [-P] xxx.fidl/xxx.fdepl] [-v] [-h]
BST IDL Code Generator
-t TYPE Set code type to be generated. Values can be: doc, capi, someip, lib, ipc, asf, test, auto.The default type is auto.
-d DIR  Set output directory, Optional, default is current directory.
-l LIC  Set license file, Optional, default is BST license.
-s      Generate stub code. By default, the lib_tool generates proxy code for client usage.This option will generate stub code for service, additionally.
-P      Generate as project module. By default, the asf_tool generates as make module.This option will generate asf code to src, include and src-gen directories.
        Only this is on, can asf gen-code add recording and simulating features. 
-v      Show version info.
-h      Show help info.
-T      Temporary features
```
### MessageBox Generator
#### Usage
&emsp;当前demo代码需配合 bst_idl_code_gen com.bst.ipc.deployment(Message Box) 1.1.0 使用，请注意检查对应生成器版本匹配。  
&emsp;note:
1. &nbsp;代码生成器路径：/opt/bstos/{version}/sysroot/x86_64-bstsdk-linux/usr/bin/idl-tools

2. &nbsp;检查版本指令：  
    ```
    bst_idl_code_gen -v
    ```
    输出信息：
    ```
    BST IDL Code Generator, compatible to :
    *    org.franca.core 0.13.1
    *    org.genivi.commonapi.core 3.2.0
    *    org.genivi.commonapi.someip 3.2.0.1
    *    com.bst.ipc.deployment (Message Box) 1.2.0
    *    commit hash 88fa1aa
    ```
3. 运行指令：
    ```
    bst_idl_code_gen xxx-server.fdepl
    bst_idl_code_gen xxx-client.fdepl
    ```
#### v1.1 release note  
1. config 协议栈部分枚举，变量命名优化  
&nbsp;注意：  
&nbsp;更新枚举原命名如下：  
    ```
    {
        IPC_MSG_TYPE_INVALID = 0,
        IPC_MSG_TYPE_METHOD,
        IPC_MSG_TYPE_REPLY,
        IPC_MSG_TYPE_BROADCAST,
        ...
    };
    ```
    更新后命名如下：
    ```
    {
        MSGBX_MSG_TYPE_INVALID = 0,
        MSGBX_MSG_TYPE_METHOD,
        MSGBX_MSG_TYPE_REPLY,
        MSGBX_MSG_TYPE_BROADCAST,
        ...
    };
    ```
2. &nbsp;config 强制开启 msgbx 驱动异常中断功能  
3. &nbsp;bugfix 应用层 token 数量增加到 256  
4. &nbsp;feature 应用层新增资源回收机制  
5. &nbsp;feature 协议栈增加快速通道接口  
6. &nbsp;feature 应用层支持定长数组  
7. &nbsp;feature 支持 selective broadcast  
8. &nbsp;feature sdk中支持代码生成器、demo code  
9. &nbsp;bugfix android 编译 error fix  
10. &nbsp;bugfix 应用层支持尺寸为 0 的 array 传输  
11. &nbsp;bugfix 序列化对齐错误  
12. &nbsp;bugfix 用户态同步调用超时问题  
13. &nbsp;bugfix 应用层fix method broadcast 没有 input 或 output 接口  
14. &nbsp;bugfix 应用层 32byte 整数倍消息长度多发一包消息  
15. &nbsp;bugfix 跨32位、64位系统 enum 字段解析异常  
16. &nbsp;performance optimization，应用层对齐序列化性能  

**特别注意：**
1. msgbx协议栈升级后，server端如果遇到枚举编译报错，需手动更新一下生成代码中相关命名。
2. msgbx代码生成器更新，对部分生成接口可能存在入参数据类型的变更，如遇编译报错，请注意对齐头文件接口更新调用方式。
  
  
#### v1.2 release note  
1. &nbsp;bugfix 应用层协议栈发送消息时增加 mutex 保证 session 串行发送
2. &nbsp;bugfix 应用层 baremetal 版本并发发送消息时，使用同一变量造成数据错乱
3. &nbsp;feature 传输层增加 session external message buffer，仅 linux kernel 中使能
4. &nbsp;feature 应用层代码结构优化
5. &nbsp;bugfix linux kernel 中同步调用阻塞被系统信号意外唤醒异常
6. &nbsp;支持 token num 调整  


## Build dependencies

```
1. A C++14 enabled compiler is needed (default for gcc >= v6.1).
2. francaidl uses CMake >= 2.10 as buildsystem.
3. francaidl uses Bison >= 1.66.0.
4. francaidl tests use Google's test framework  
https://code.google.com/p/googletest/[gtest] 
-- URL: https://googletest.googlecode.com/files/gtest-.zip
```

## Build from source code

```
mkdir build
cd build
cmake ..
make
```

## Used third-party tools
- [fmt: A modern formatting library](https://github.com/fmtlib/fmt) which is licensed under the MIT, is used by the francaidl project to process the strings.
- [nlohmann/json](https://github.com/nlohmann/json) which is licensed under the MIT, is used by the asf generator and test generator in the francaidl project.

## License

```
The Project francaidl is licensed under [Apache-2.0](LICENSE)
```