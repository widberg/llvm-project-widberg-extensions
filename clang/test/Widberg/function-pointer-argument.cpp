// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" ptr @_Z11call_fn_ptrPU8usercallFPilE(ptr noundef "widberg_location"="edx" %x)
int *__usercall call_fn_ptr@<ebx>(int *(__usercall *x)@<eax>(long @<ecx>)@<edx>) { 
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(i32 noundef "widberg_location"="ecx" 1337)
    return x(1337);
}
