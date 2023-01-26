# The LLVM Compiler Infrastructure With Widberg Extensions

This repository is a fork of LLVM intended to implement C/C++ language
features in LLVM/Clang to aid in reverse engineering. Currently, the
scope of this project covers a subset of the IDA Pro __usercall
syntax. An example of the syntax that is currently supported is as follows:

```cpp
long long __usercall __spoils<eax,esi>
square@<ebx:ecx>(long long num@<eax:edx>) {
    return num * num;
}

bool __usercall is_even@<al>(int num) {
    return num % 2 == 0;
}

void __userpurge is_odd(int num, bool &result@<eax>) {
    result = num % 2 == 1;
}

auto is_odd_also = is_odd;

int *__usercall call_fn_ptr@<ebx>(int *(__usercall *x)@<eax>(long @<ecx>)@<edx>) {
    return x(1337);
}
```

The first thing most people coming from MSVC say to me when I tell them
about this project is, "I won't have to do the __fastcall/__thiscall trick
anymore." What they don't know is that Clang already allows __thiscall on
non-member functions without this fork. For example the following is
acceptable in mainline Clang as well as this fork and produces the correct
output (_this in ecx, other args on the stack):

```cpp
int __thiscall square(void *_this, int num) {
    return num * num;
}
```

The project is functional but lacks polish. Correct syntax will be accepted
and generate correct code; however, incorrect syntax is handled largely by
asserts and internal compiler errors. More work needs to be done to take
advantage of Clang's diagnostics infrastructure and produce pretty errors
rather than compiler stack traces. Additionally, some incorrect syntax is
accepted and ignored rather than reported.

Currently, only the X86_32 and X86_64 backends are supported in an
effort to limit the scope of the project while it is early in development.
As things stabilize more backends will be suppoerted. This limitation is
entirely self-imposed and can be easily removed when the time is right.

Next steps are to improve the diagnostics reporting as described above
and add unit tests.

Pull requests and issues are encouraged.

## Enable and Disable the Extensions

By default, the extensions are enabled. They can be disabled using the
Clang option `-fno-widberg-extensions`.

## Verify Widberg Extensions Are Present

The following construct can be used in source files to verify that the
widberg extensions are present:

```cpp
#ifndef __has_feature
#  define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif
#ifndef __has_extension
#  define __has_extension __has_feature // Compatibility with pre-3.0 compilers.
#endif

#if !__has_extension(widberg)
#  error "This file requires a compiler that implements the widberg extensions."
#endif
```

## Compiler Explorer

The compiler is available on the [Compiler Explorer website](https://godbolt.org/z/j4dPsE8rq).

The companion repository [widberg/compiler-explorer-widberg](
https://github.com/widberg/compiler-explorer-widberg) contains a
compiler-explorer configuration for this Clang driver as well as a
`run.sh` script to quickly launch the local compiler-explorer instance
with the correct options. This is useful for quickly prototyping the
compiler.

# Affiliation with LLVM (Or Lack Thereof)

This project is not affiliated with the LLVM project in any way.
This project, like the LLVM project, is under the Apache License
v2.0 with LLVM Exceptions. I have no intention of upstreaming any
of the changes made in this repository as I believe they are not
useful to most people. The original LLVM project README.md begins
below.

# The LLVM Compiler Infrastructure

This directory and its sub-directories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and run-time environments.

The README briefly describes how to get started with building LLVM.
For more information on how to contribute to the LLVM project, please
take a look at the
[Contributing to LLVM](https://llvm.org/docs/Contributing.html) guide.

## Getting Started with the LLVM System

Taken from https://llvm.org/docs/GettingStarted.html.

### Overview

Welcome to the LLVM project!

The LLVM project has multiple components. The core of the project is
itself called "LLVM". This contains all of the tools, libraries, and header
files needed to process intermediate representations and convert them into
object files.  Tools include an assembler, disassembler, bitcode analyzer, and
bitcode optimizer.  It also contains basic regression tests.

C-like languages use the [Clang](http://clang.llvm.org/) front end.  This
component compiles C, C++, Objective-C, and Objective-C++ code into LLVM bitcode
-- and from there into object files, using LLVM.

Other components include:
the [libc++ C++ standard library](https://libcxx.llvm.org),
the [LLD linker](https://lld.llvm.org), and more.

### Getting the Source Code and Building LLVM

The LLVM Getting Started documentation may be out of date.  The [Clang
Getting Started](http://clang.llvm.org/get_started.html) page might have more
accurate information.

This is an example work-flow and configuration to get and build the LLVM source:

1. Checkout LLVM (including related sub-projects like Clang):

     * ``git clone https://github.com/llvm/llvm-project.git``

     * Or, on windows, ``git clone --config core.autocrlf=false
    https://github.com/llvm/llvm-project.git``

2. Configure and build LLVM and Clang:

     * ``cd llvm-project``

     * ``cmake -S llvm -B build -G <generator> [options]``

        Some common build system generators are:

        * ``Ninja`` --- for generating [Ninja](https://ninja-build.org)
          build files. Most llvm developers use Ninja.
        * ``Unix Makefiles`` --- for generating make-compatible parallel makefiles.
        * ``Visual Studio`` --- for generating Visual Studio projects and
          solutions.
        * ``Xcode`` --- for generating Xcode projects.

        Some common options:

        * ``-DLLVM_ENABLE_PROJECTS='...'`` and ``-DLLVM_ENABLE_RUNTIMES='...'`` ---
          semicolon-separated list of the LLVM sub-projects and runtimes you'd like to
          additionally build. ``LLVM_ENABLE_PROJECTS`` can include any of: clang,
          clang-tools-extra, cross-project-tests, flang, libc, libclc, lld, lldb,
          mlir, openmp, polly, or pstl. ``LLVM_ENABLE_RUNTIMES`` can include any of
          libcxx, libcxxabi, libunwind, compiler-rt, libc or openmp. Some runtime
          projects can be specified either in ``LLVM_ENABLE_PROJECTS`` or in
          ``LLVM_ENABLE_RUNTIMES``.

          For example, to build LLVM, Clang, libcxx, and libcxxabi, use
          ``-DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi"``.

        * ``-DCMAKE_INSTALL_PREFIX=directory`` --- Specify for *directory* the full
          path name of where you want the LLVM tools and libraries to be installed
          (default ``/usr/local``). Be careful if you install runtime libraries: if
          your system uses those provided by LLVM (like libc++ or libc++abi), you
          must not overwrite your system's copy of those libraries, since that
          could render your system unusable. In general, using something like
          ``/usr`` is not advised, but ``/usr/local`` is fine.

        * ``-DCMAKE_BUILD_TYPE=type`` --- Valid options for *type* are Debug,
          Release, RelWithDebInfo, and MinSizeRel. Default is Debug.

        * ``-DLLVM_ENABLE_ASSERTIONS=On`` --- Compile with assertion checks enabled
          (default is Yes for Debug builds, No for all other build types).

      * ``cmake --build build [-- [options] <target>]`` or your build system specified above
        directly.

        * The default target (i.e. ``ninja`` or ``make``) will build all of LLVM.

        * The ``check-all`` target (i.e. ``ninja check-all``) will run the
          regression tests to ensure everything is in working order.

        * CMake will generate targets for each tool and library, and most
          LLVM sub-projects generate their own ``check-<project>`` target.

        * Running a serial build will be **slow**.  To improve speed, try running a
          parallel build.  That's done by default in Ninja; for ``make``, use the option
          ``-j NNN``, where ``NNN`` is the number of parallel jobs, e.g. the number of
          CPUs you have.

      * For more information see [CMake](https://llvm.org/docs/CMake.html)

Consult the
[Getting Started with LLVM](https://llvm.org/docs/GettingStarted.html#getting-started-with-llvm)
page for detailed information on configuring and compiling LLVM. You can visit
[Directory Layout](https://llvm.org/docs/GettingStarted.html#directory-layout)
to learn about the layout of the source code tree.
