// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

int __usercall good_return_pre@<eax>(int x);
int __usercall good_fnptr_param@<ebx>(
    int (__usercall *x)@<eax>(int @<ecx>)@<edx>);

void __usercall bad_function_post_void(int x)@<eax>; // expected-error {{post-parameter Widberg location is only valid on function pointer parameters}}
int bad_function_post_default(int x)@<eax>; // expected-error {{post-parameter Widberg location is only valid on function pointer parameters}}
int __usercall bad_function_post_both@<eax>(int x)@<edx>; // expected-error {{post-parameter Widberg location is only valid on function pointer parameters}}
