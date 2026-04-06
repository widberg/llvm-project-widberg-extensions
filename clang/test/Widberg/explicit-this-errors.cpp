// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

struct A {
  int __usercall ok@<eax>(this A &self);
  static int __usercall GetID@<eax>(int ContinueCRC@<eax>, const char *_Name@<edx>);

  int __usercall missing_usercall@<eax>(); // expected-error {{non-static member function with usercall calling convention must declare an explicit object parameter}}
  int __userpurge missing_userpurge@<eax>(); // expected-error {{non-static member function with userpurge calling convention must declare an explicit object parameter}}
};

int __usercall A::ok@<eax>(this A &self) {
  return 0;
}

int __usercall A::GetID@<eax>(int ContinueCRC@<eax>, const char *_Name@<edx>) {
  return 0;
}

int __usercall A::missing_usercall@<eax>() { // expected-error {{non-static member function with usercall calling convention must declare an explicit object parameter}}
  return 1;
}

int __userpurge A::missing_userpurge@<eax>() { // expected-error {{non-static member function with userpurge calling convention must declare an explicit object parameter}}
  return 2;
}
