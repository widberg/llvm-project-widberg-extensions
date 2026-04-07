// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

int __usercall missing_usercall(int x); // expected-error {{non-void function with usercall calling convention must specify a return location}}
int __userpurge missing_userpurge(int x); // expected-error {{non-void function with userpurge calling convention must specify a return location}}

void __usercall ok_usercall_void(int x);
void __userpurge ok_userpurge_void(int x);

int __usercall present_usercall@<eax>(int x);
int __userpurge present_userpurge@<eax>(int x);
