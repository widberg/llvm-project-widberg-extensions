# The LLVM Compiler Infrastructure With Widberg Extensions

The LLVM Compiler Infrastructure With Widberg Extensions, affectionately called
the Widpiler by many, is a fork of LLVM intended to implement C/C++ language features in
LLVM/Clang to aid in reverse engineering. Currently, the scope of this project
covers a subset of the IDA Pro [__usercall syntax](https://github.com/widberg/llvm-project-widberg-extensions/wiki/User‐Defined-Calling-Conventions)
and [shifted pointers](https://github.com/widberg/llvm-project-widberg-extensions/wiki/Shifted-Pointers).
This is a research project and not production ready. Please report any issues to
this repository and not to upstream LLVM.

[![Build Status](https://github.com/widberg/llvm-project-widberg-extensions/actions/workflows/widberg-build.yml/badge.svg?branch=main)](https://github.com/widberg/llvm-project-widberg-extensions/actions/workflows/widberg-build.yml)
[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/widberg/llvm-project-widberg-extensions)](https://github.com/widberg/llvm-project-widberg-extensions/releases)
[![Release Nightly](https://img.shields.io/badge/release-nightly-5e025f?labelColor=301934)](https://nightly.link/widberg/llvm-project-widberg-extensions/workflows/widberg-build/main)

## Example

An example of the syntax that is currently supported is as follows:

```cpp
// __usercall
// https://hex-rays.com/blog/igors-tip-of-the-week-51-custom-calling-conventions
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

// __shifted
// https://hex-rays.com/blog/igors-tip-of-the-week-54-shifted-pointers
// https://hex-rays.com/blog/igors-tip-of-the-week-57-shifted-pointers-2
typedef struct vec3f {
    float x;
    float y;
    float z;
} vec3f_t;

typedef struct player {
    char name[16];
    int health;
    int armor;
    int ammo;
    vec3f_t pos;
} player_t;

const char *get_player_name_from_shifted_pos_pointer(const vec3f_t *__shifted(player_t, 0x1C) pos) {
    return ADJ(pos)->name;
}
```

The first thing most people coming from MSVC say to me when I tell them
about this project is, "I won't have to do the `__fastcall`/`__thiscall` trick
anymore." What they don't know is that Clang already allows `__thiscall` on
non-member functions without this fork. For example the following is
acceptable in mainline Clang, as well as this fork, and produces the correct
output (`_this` in `ecx`, other args on the stack):

```cpp
int __thiscall square(void *_this, int num) {
    return num * num;
}
```

This project also adds some MSVC compatibility features not found in LLVM:
* Allow [`__FUNCDNAME__`, `__FUNCSIG__`, and `__FUNCTION__`](https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170) to be used in `pragma`s.
* Add [`#pragma comment(user, "...")`](https://learn.microsoft.com/en-us/cpp/preprocessor/comment-c-cpp?view=msvc-170).

I'll also point out [backengineering/llvm-msvc](https://github.com/backengineering/llvm-msvc). It has a lot more MSVC compatibility improvements. If your use case is sufficiently complicated, then you might want to cherry-pick some of the improvements from that repository into here.

## Compiler Explorer

The compiler is available on the [Compiler Explorer website](https://godbolt.org/z/9TdsYGqG9).

## Motivation

The goal of the project is not to recompile decomplied code in all cases, but rather to provide a familiar syntax for common patterns and reduce the amount of inline assembly and fiddling required when writing high-level patch code. However, by providing this syntax it is possible to recompile decompiled code in some cases. With the addition of [defs.h](#defsh) most individual functions can be recompiled with little to no modification. Recompiling an entire binary will still require great effort, but is made easier by this project.

## Status

The project is semi-functional but lacks polish. Correct syntax will be accepted
and generate correct code in most cases; however, incorrect syntax is handled
largely by asserts and internal compiler errors, especially in X86_64. More work
needs to be done to take advantage of Clang's diagnostics infrastructure and
produce pretty errors rather than compiler stack traces. Additionally, some
incorrect syntax is accepted and ignored rather than reported. Currently, only
the X86_32 and X86_64 backends are supported.

Next steps are to improve the diagnostics reporting as described above and fix
the bugs. Pull requests and issues are encouraged; especially pull requests
adding tests.

Furthermore, scattered argument locations haven't been implemented yet and the
name mangling scheme for `__usercall`/`__userpurge` function symbols,
especially important when interacting with `extern "C"`, have not yet been
finalized. These are lower priority since they are easily worked around.

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

Also, the preprocessor macro `__widberg__` is predefined if the extensions are present.

## defs.h

An alternative implementation of defs.h from the Hex-Rays decompiler sdk intended to be used with this project can be found at https://github.com/widberg/widberg-defs. A lot of the stuff in there is overkill for writing patch code, but it is useful for recompiling decompiled code.

## Build

Use `x64 Native Tools Command Prompt`

```sh
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld" -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --config RelWithDebInfo --target clang clangd lld
```

# Affiliation with LLVM (Or Lack Thereof)

This project is not affiliated with the LLVM project in any way.
This project, like the LLVM project, is under the Apache License
v2.0 with LLVM Exceptions. I have no intention of upstreaming any
of the changes made in this repository as I believe they are not
useful to most people. The original LLVM project README.md begins
below.

# The LLVM Compiler Infrastructure

[![OpenSSF Scorecard](https://api.securityscorecards.dev/projects/github.com/llvm/llvm-project/badge)](https://securityscorecards.dev/viewer/?uri=github.com/llvm/llvm-project)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/8273/badge)](https://www.bestpractices.dev/projects/8273)
[![libc++](https://github.com/llvm/llvm-project/actions/workflows/libcxx-build-and-test.yaml/badge.svg?branch=main&event=schedule)](https://github.com/llvm/llvm-project/actions/workflows/libcxx-build-and-test.yaml?query=event%3Aschedule)

Welcome to the LLVM project!

This repository contains the source code for LLVM, a toolkit for the
construction of highly optimized compilers, optimizers, and run-time
environments.

The LLVM project has multiple components. The core of the project is
itself called "LLVM". This contains all of the tools, libraries, and header
files needed to process intermediate representations and convert them into
object files. Tools include an assembler, disassembler, bitcode analyzer, and
bitcode optimizer.

C-like languages use the [Clang](https://clang.llvm.org/) frontend. This
component compiles C, C++, Objective-C, and Objective-C++ code into LLVM bitcode
-- and from there into object files, using LLVM.

Other components include:
the [libc++ C++ standard library](https://libcxx.llvm.org),
the [LLD linker](https://lld.llvm.org), and more.

## Getting the Source Code and Building LLVM

Consult the
[Getting Started with LLVM](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm)
page for information on building and running LLVM.

For information on how to contribute to the LLVM project, please take a look at
the [Contributing to LLVM](https://llvm.org/docs/Contributing.html) guide.

## Getting in touch

Join the [LLVM Discourse forums](https://discourse.llvm.org/), [Discord
chat](https://discord.gg/xS7Z362),
[LLVM Office Hours](https://llvm.org/docs/GettingInvolved.html#office-hours) or
[Regular sync-ups](https://llvm.org/docs/GettingInvolved.html#online-sync-ups).

The LLVM project has adopted a [code of conduct](https://llvm.org/docs/CodeOfConduct.html) for
participants to all modes of communication within the project.
