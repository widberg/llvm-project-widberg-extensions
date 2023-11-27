// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx,ecx" i64 @_Z6squarex(i64 noundef "widberg_location"="eax,edx" %num)
long long __usercall
square@<ebx:ecx>(long long num@<eax:edx>) {
    return num * num;
}
