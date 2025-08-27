// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define{{.*}} usercallcc{{.*}} "widberg_location"="al" i1 @_Z7is_eveni(i32 noundef %num)
bool __usercall is_even@<al>(int num) {
    return num % 2 == 0;
}
