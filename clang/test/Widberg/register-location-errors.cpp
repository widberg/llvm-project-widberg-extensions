// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

int __usercall bad_name@<nope>(int x); // expected-error {{invalid register name 'nope' in Widberg location}}
int __usercall bad_rax_i386@<rax>(int x); // expected-error {{invalid register name 'rax' in Widberg location}}
int __usercall bad_r8_i386@<r8>(int x); // expected-error {{invalid register name 'r8' in Widberg location}}
int __usercall bad_r8d_i386@<r8d>(int x); // expected-error {{invalid register name 'r8d' in Widberg location}}
double __usercall bad_xmm8_i386@<xmm8>(double x); // expected-error {{invalid register name 'xmm8' in Widberg location}}
double __usercall bad_fp8@<fp8>(double x); // expected-error {{invalid register name 'fp8' in Widberg location}}
double __usercall bad_st@<st>(double x); // expected-error {{invalid register name 'st' in Widberg location}}
double __usercall bad_st0@<st0>(double x); // expected-error {{invalid register name 'st0' in Widberg location}}
double __usercall bad_st8@<st8>(double x); // expected-error {{invalid register name 'st8' in Widberg location}}
double __usercall ok_fp0@<fp0>(double x);

int __usercall bad_ret_i32@<al>(int x); // expected-error {{register 'al' in Widberg location for return value is incompatible with type}}
bool __usercall bad_ret_bool@<eax>(bool x); // expected-error {{register 'eax' in Widberg location for return value is incompatible with type}}
long long __usercall bad_ret_i64_eax@<eax>(long long x); // expected-error {{widberg location for return value requires at least 2 registers}}
long long __usercall bad_ret_i64_xmm0@<xmm0>(long long x); // expected-error {{widberg location for return value requires at least 2 registers}}
double __usercall bad_ret_double_eax@<eax>(double x); // expected-error {{register 'eax' in Widberg location for return value is incompatible with type}}
int __usercall bad_ret_extra_regs@<eax:edx>(int x); // expected-error {{register 'edx' in Widberg location for return value is incompatible with type}}

int __usercall bad_param_i32@<eax>(int x@<al>); // expected-error {{register 'al' in Widberg location for parameter is incompatible with type}}
bool __usercall bad_param_bool@<al>(bool x@<eax>); // expected-error {{register 'eax' in Widberg location for parameter is incompatible with type}}
long long __usercall bad_param_i64_eax@<eax:edx>(long long x@<eax>); // expected-error {{widberg location for parameter requires at least 2 registers}}
long long __usercall bad_param_i64_xmm0@<eax:edx>(long long x@<xmm0>); // expected-error {{widberg location for parameter requires at least 2 registers}}
long long __usercall bad_param_split@<eax:edx>(long long x@<eax:al>); // expected-error {{register 'al' in Widberg location for parameter is incompatible with type}}
int __usercall bad_param_extra_regs@<eax>(int x@<eax:edx>); // expected-error {{register 'edx' in Widberg location for parameter is incompatible with type}}

struct A {
  int X;
};
int __usercall bad_ref_param@<eax>(A &a@<al>); // expected-error {{register 'al' in Widberg location for parameter is incompatible with type}}
decltype(nullptr) __usercall bad_nullptr_ret@<al>(int x); // expected-error {{register 'al' in Widberg location for return value is incompatible with type}}

double __usercall bad_st_paren_name@<st(0)>(int x); // expected-error {{all Widberg location entries must be identifiers}} expected-error {{non-void function with usercall calling convention must specify a return location}}
int __usercall bad_empty_ret@<>(int x); // expected-error {{widberg location for return value must contain at least one register}}
int __usercall bad_empty_param@<eax>(int x@<>); // expected-error {{widberg location for parameter must contain at least one register}}
int bad_default_cc_ret@<eax>(int x); // expected-error {{widberg location for return value requires 'usercall' or 'userpurge' calling convention}}
int bad_default_cc_param(int x@<eax>); // expected-error {{widberg location for parameter requires 'usercall' or 'userpurge' calling convention}}
int __stdcall bad_stdcall_cc_ret@<eax>(int x); // expected-error {{widberg location for return value requires 'usercall' or 'userpurge' calling convention}}
int __stdcall bad_stdcall_cc_param(int x@<eax>); // expected-error {{widberg location for parameter requires 'usercall' or 'userpurge' calling convention}}
int *__usercall ok_nested_fp@<eax>(int *(__usercall *x)@<ecx>(long @<edx>));
int bad_nested_fp(int (*x)@<eax>(long @<ecx>)); // expected-error {{widberg location for return value requires 'usercall' or 'userpurge' calling convention}} expected-error {{widberg location for parameter requires 'usercall' or 'userpurge' calling convention}}
int bad_nested_fp_malformed(int (*x)@<st(0)>(long)); // expected-error {{all Widberg location entries must be identifiers}}

long long __usercall ok_i64@<eax:edx>(long long x);
int __usercall ok_i32@<eax>(int x);
float __usercall ok_float@<ecx>(float x);
