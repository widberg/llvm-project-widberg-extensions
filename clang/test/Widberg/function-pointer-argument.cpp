// RUN: %clang_cc1 -triple i386-unknown-unknown -emit-llvm -o - %s | FileCheck --check-prefix=CHECK-LABEL %s

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" ptr @_Z11call_fn_ptrPU8usercallFPilE(ptr noundef "widberg_location"="edx" %x)
int *__usercall call_fn_ptr@<ebx>(int *(__usercall *x)@<eax>(long @<ecx>)@<edx>) { 
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(i32 noundef "widberg_location"="ecx" 1337)
    return x(1337);
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" ptr @_Z{{.*}}(ptr noundef "widberg_location"="edx" %x) [[TF3:#[0-9]+]] {
int *__usercall __spoils<eax,esi> call_fn_ptr_spoils@<ebx>(int *(__usercall __spoils<ecx> *x)@<eax>(long @<ecx>)@<edx>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(i32 noundef "widberg_location"="ecx" 1337) [[CS3:#[0-9]+]]
    return x(1337);
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" i32 @_Z{{.*}}(ptr noundef "widberg_location"="edx" %x)
int __usercall call_fn_ptr_i32@<ebx>(int (__usercall *x)@<eax>(long @<ecx>)@<edx>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" i32 %0(i32 noundef "widberg_location"="ecx" 1337)
    return x(1337);
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ecx" ptr [[LEAF:@_Z[^ (]+]](i32 noundef "widberg_location"="esi" %x)
int *__usercall leaf_fn_ptr@<ecx>(long x@<esi>) {
    return nullptr;
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" ptr @_Z{{.*}}(ptr noundef "widberg_location"="edi" %x)
int *__usercall call_fn_ptr_arg_fn_ptr@<ebx>(
    int *(__usercall *x)@<eax>(int *(__usercall *y)@<ecx>(long @<esi>)@<edx>)@<edi>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(ptr noundef "widberg_location"="edx" [[LEAF]])
    return x(leaf_fn_ptr);
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ecx" ptr [[MID:@_Z[^ (]+]](ptr noundef "widberg_location"="edx" %z)
int *__usercall mid_fn_ptr@<ecx>(int *(__usercall *z)@<esi>(long @<edi>)@<edx>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="esi" ptr %0(i32 noundef "widberg_location"="edi" 1337)
    return z(1337);
}

// CHECK-LABEL: define{{.*}} usercallcc noundef "widberg_location"="ebx" ptr @_Z{{.*}}(ptr noundef "widberg_location"="edi" %x)
int *__usercall call_fn_ptr_arg_fn_ptr_deeper@<ebx>(
    int *(__usercall *x)@<eax>(
        int *(__usercall *y)@<ecx>(
            int *(__usercall *z)@<esi>(long @<edi>)@<edx>
        )@<edx>
    )@<edi>) {
    // CHECK-LABEL: %call = call usercallcc noundef "widberg_location"="eax" ptr %0(ptr noundef "widberg_location"="edx" [[MID]])
    return x(mid_fn_ptr);
}

// CHECK-LABEL: attributes [[TF3]] = { {{.*}}"spoils"="eax,esi"{{.*}} }
// CHECK-LABEL: attributes [[CS3]] = { {{.*}}"spoils"="ecx"{{.*}} }
