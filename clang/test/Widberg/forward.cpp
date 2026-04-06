// RUN: %clang_cc1 -triple i386-pc-win32-msvc -std=c++17 -O3 -Wall -Werror -emit-pch -o %t %s
// RUN: %clang_cc1 -triple i386-pc-win32-msvc -std=c++17 -O3 -Wall -Werror -include-pch %t -verify %s

#ifndef HEADER
#define HEADER

int __userpurge foo@<ebx>(int* a1@<edi>, int a2);
int __userpurge bar@<eax>(int* a1@<edx>, int a2);
int __usercall baz@<ecx>(int a1);
int __usercall qux(int a1);

#else

int __userpurge foo@<ebx>(int* a1@<esi>, int a2) { // expected-error{{conflicting types for 'foo'}}
    return *a1 + a2;
}
// expected-note@7{{previous declaration is here}}

int __userpurge bar@<eax>(int* a1@<edx>, int a2) {
    return *a1 + a2;
}

int __usercall baz(int a1) { // expected-error{{conflicting types for 'baz'}}
    return a1;
}
// expected-note@9{{previous declaration is here}}

int __usercall qux@<ecx>(int a1) { // expected-error{{conflicting types for 'qux'}}
    return a1;
}
// expected-note@10{{previous declaration is here}}

#endif
