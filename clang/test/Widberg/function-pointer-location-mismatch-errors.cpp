// RUN: %clang_cc1 -triple i386-pc-windows-msvc -fsyntax-only -verify %s

typedef int (__usercall *FnRetEax)@<eax>(int @<ecx>);
typedef int (__usercall *FnRetEbx)@<ebx>(int @<ecx>);

int __usercall fn_ret_eax@<eax>(int @<ecx>);
int __usercall fn_ret_ebx@<ebx>(int @<ecx>);

void takes_fn_ret_eax(FnRetEax);
// expected-note@-1{{candidate function not viable: no known conversion from 'FnRetEbx'}}

void test_top_level_mismatch() {
  FnRetEax ok = fn_ret_eax;
  FnRetEbx rhs = fn_ret_ebx;
  FnRetEax bad = rhs; // expected-error {{cannot initialize a variable of type 'FnRetEax'}}
  ok = rhs; // expected-error {{incompatible function pointer types assigning to 'FnRetEax'}}

  takes_fn_ret_eax(fn_ret_eax);
  takes_fn_ret_eax(rhs); // expected-error {{no matching function for call to 'takes_fn_ret_eax'}}
}

typedef int (__usercall *LeafParamEcx)@<eax>(int @<ecx>);
typedef int (__usercall *LeafParamEdx)@<eax>(int @<edx>);

typedef int (__usercall *MidLeafEcx)@<ebx>(LeafParamEcx @<esi>);
typedef int (__usercall *MidLeafEdx)@<ebx>(LeafParamEdx @<esi>);

int __usercall mid_leaf_ecx@<ebx>(LeafParamEcx @<esi>);
int __usercall mid_leaf_edx@<ebx>(LeafParamEdx @<esi>);

void takes_mid_leaf_ecx(MidLeafEcx);
// expected-note@-1{{candidate function not viable: no known conversion from 'MidLeafEdx'}}

void test_nested_mismatch() {
  MidLeafEcx ok = mid_leaf_ecx;
  MidLeafEdx rhs = mid_leaf_edx;
  MidLeafEcx bad = rhs; // expected-error {{cannot initialize a variable of type 'MidLeafEcx'}}
  ok = rhs; // expected-error {{incompatible function pointer types assigning to 'MidLeafEcx'}}

  takes_mid_leaf_ecx(mid_leaf_ecx);
  takes_mid_leaf_ecx(rhs); // expected-error {{no matching function for call to 'takes_mid_leaf_ecx'}}
}

typedef int (__usercall *DeepMidLeafEcx)@<edx>(MidLeafEcx @<edi>);
typedef int (__usercall *DeepMidLeafEdx)@<edx>(MidLeafEdx @<edi>);

int __usercall deep_mid_leaf_ecx@<edx>(MidLeafEcx @<edi>);
int __usercall deep_mid_leaf_edx@<edx>(MidLeafEdx @<edi>);

void takes_deep_mid_leaf_ecx(DeepMidLeafEcx);
// expected-note@-1{{candidate function not viable: no known conversion from 'DeepMidLeafEdx'}}

void test_deeper_nested_mismatch() {
  DeepMidLeafEcx ok = deep_mid_leaf_ecx;
  DeepMidLeafEdx rhs = deep_mid_leaf_edx;
  DeepMidLeafEcx bad = rhs; // expected-error {{cannot initialize a variable of type 'DeepMidLeafEcx'}}
  ok = rhs; // expected-error {{incompatible function pointer types assigning to 'DeepMidLeafEcx'}}

  takes_deep_mid_leaf_ecx(deep_mid_leaf_ecx);
  takes_deep_mid_leaf_ecx(rhs); // expected-error {{no matching function for call to 'takes_deep_mid_leaf_ecx'}}
}

typedef int (__usercall *LeafRetEax)@<eax>(int @<ecx>);
typedef int (__usercall *LeafRetEbx)@<ebx>(int @<ecx>);

typedef int (__usercall *MidRetEax)@<edx>(LeafRetEax @<esi>);
typedef int (__usercall *MidRetEbx)@<edx>(LeafRetEbx @<esi>);

int __usercall mid_ret_eax@<edx>(LeafRetEax @<esi>);
int __usercall mid_ret_ebx@<edx>(LeafRetEbx @<esi>);

void takes_mid_ret_eax(MidRetEax);
// expected-note@-1{{candidate function not viable: no known conversion from 'MidRetEbx'}}

void test_nested_return_location_mismatch() {
  MidRetEax ok = mid_ret_eax;
  MidRetEbx rhs = mid_ret_ebx;
  MidRetEax bad = rhs; // expected-error {{cannot initialize a variable of type 'MidRetEax'}}
  ok = rhs; // expected-error {{incompatible function pointer types assigning to 'MidRetEax'}}

  takes_mid_ret_eax(mid_ret_eax);
  takes_mid_ret_eax(rhs); // expected-error {{no matching function for call to 'takes_mid_ret_eax'}}
}

typedef int (__usercall *DeepRetEax)@<ecx>(MidRetEax @<edi>);
typedef int (__usercall *DeepRetEbx)@<ecx>(MidRetEbx @<edi>);

int __usercall deep_ret_eax@<ecx>(MidRetEax @<edi>);
int __usercall deep_ret_ebx@<ecx>(MidRetEbx @<edi>);

void takes_deep_ret_eax(DeepRetEax);
// expected-note@-1{{candidate function not viable: no known conversion from 'DeepRetEbx'}}

void test_deeper_nested_return_location_mismatch() {
  DeepRetEax ok = deep_ret_eax;
  DeepRetEbx rhs = deep_ret_ebx;
  DeepRetEax bad = rhs; // expected-error {{cannot initialize a variable of type 'DeepRetEax'}}
  ok = rhs; // expected-error {{incompatible function pointer types assigning to 'DeepRetEax'}}

  takes_deep_ret_eax(deep_ret_eax);
  takes_deep_ret_eax(rhs); // expected-error {{no matching function for call to 'takes_deep_ret_eax'}}
}
