# Franca IDL
The francaidl support a Franca IDL parser which can parse IDL files and build complete model trees. And it also support a IDL code generation tool that can generate various types of code based on the model trees.  
It is independent of the operating system, can be compatible with multiple operating systems, and can currently run on the BST-OS linux, Windows, Ubuntu 18.04, Ubuntu 20.04 or Ubuntu 22.04 operating system.

### Features
- IDL code generator : A C++ code generator, which parsing the given IDL files and then generating the appropriate  code according to the specified type or deployment in the IDL files.

### Usage

#### IDL code generator
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

### Build dependencies

```
1. A C++14 enabled compiler is needed (default for gcc >= v6.1).
2. francaidl uses CMake >= 2.10 as buildsystem.
3. francaidl uses Bison >= 1.66.0.
4. francaidl tests use Google's test framework  
https://code.google.com/p/googletest/[gtest] 
-- URL: https://googletest.googlecode.com/files/gtest-.zip
```

### Build from source code

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