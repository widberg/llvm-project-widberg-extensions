// RUN: grep -Ev "// *[A-Z-]+:" %s | clang-format -style=LLVM \
// RUN:   | FileCheck -strict-whitespace %s

// CHECK: {{^unsigned\ long\ long\ __usercall\ __spoils<eax,edx>}}
// CHECK-NEXT: {{test@<eax:edx>\(int\ \*i@<eax:edx>\);}}
 unsigned  long long   __usercall    __spoils <  eax ,  edx >  test  @ <eax : edx > ( int *  i   @  <eax: edx>  ) ;
