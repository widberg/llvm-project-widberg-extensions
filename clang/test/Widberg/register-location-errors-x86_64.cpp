// RUN: %clang_cc1 -triple x86_64-pc-windows-msvc -fsyntax-only -verify %s

// x86_64-only register spellings should be accepted when widths match.
long long __usercall ok_ret_i64_rax@<rax>(long long x);
int __usercall ok_ret_i32_r8d@<r8d>(int x);
bool __usercall ok_ret_bool_r8b@<r8b>(bool x);
double __usercall ok_ret_double_xmm8@<xmm8>(double x);
long long __usercall ok_param_i64_r8@<rax>(long long x@<r8>);
int *__usercall ok_param_ptr_r9@<rax>(int *x@<r9>);
decltype(nullptr) __usercall ok_nullptr_ret_rax@<rax>(int x);

// Out-of-range and legacy alias spellings should still be rejected.
int __usercall bad_r32_name@<r32>(int x); // expected-error {{invalid register name 'r32' in Widberg location}}
double __usercall bad_st0_alias@<st0>(double x); // expected-error {{invalid register name 'st0' in Widberg location}}

// Under x86_64, integer-like 64-bit values require a single 64-bit GPR.
long long __usercall bad_ret_i64_eax@<eax>(long long x); // expected-error {{register 'eax' in Widberg location for return value is incompatible with type}}
long long __usercall bad_ret_i64_extra@<rax:rdx>(long long x); // expected-error {{register 'rdx' in Widberg location for return value is incompatible with type}}
long long __usercall bad_param_i64_eax@<rax>(long long x@<eax>); // expected-error {{register 'eax' in Widberg location for parameter is incompatible with type}}
long long __usercall bad_param_i64_extra32@<rax>(long long x@<eax:edx>); // expected-error {{register 'edx' in Widberg location for parameter is incompatible with type}} expected-error {{register 'eax' in Widberg location for parameter is incompatible with type}}

// 64-bit pointer-sized entities reject 32-bit GPRs on x86_64.
int *__usercall bad_ret_ptr_eax@<eax>(int *x); // expected-error {{register 'eax' in Widberg location for return value is incompatible with type}}
int *__usercall bad_param_ptr_eax@<rax>(int *x@<eax>); // expected-error {{register 'eax' in Widberg location for parameter is incompatible with type}}
decltype(nullptr) __usercall bad_nullptr_ret_eax@<eax>(int x); // expected-error {{register 'eax' in Widberg location for return value is incompatible with type}}

// High registers are valid names on x86_64, so type checks should fire.
int __usercall bad_ret_i32_r8@<r8>(int x); // expected-error {{register 'r8' in Widberg location for return value is incompatible with type}}
int __usercall bad_ret_i32_xmm8@<xmm8>(int x); // expected-error {{register 'xmm8' in Widberg location for return value is incompatible with type}}
bool __usercall bad_ret_bool_r8d@<r8d>(bool x); // expected-error {{register 'r8d' in Widberg location for return value is incompatible with type}}
