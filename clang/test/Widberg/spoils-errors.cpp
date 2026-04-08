// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

int __spoils<none> bad_name(int x); // expected-error {{invalid register name 'none' in __spoils list}}
int __spoils<rax> bad_rax_i386(int x); // expected-error {{invalid register name 'rax' in __spoils list}}
int __spoils<r8> bad_r8_i386(int x); // expected-error {{invalid register name 'r8' in __spoils list}}
int __spoils<fp8> bad_fp8(int x); // expected-error {{invalid register name 'fp8' in __spoils list}}
double __spoils<st0> bad_st0(double x); // expected-error {{invalid register name 'st0' in __spoils list}}
int __spoils<eax,none,edx> bad_mixed(int x); // expected-error {{invalid register name 'none' in __spoils list}}

int __spoils<eax,edx> ok_gprs(int x);
double __spoils<fp0> ok_fp0(double x);
